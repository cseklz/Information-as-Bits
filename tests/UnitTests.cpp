#include "AsciiConverter.h"
#include "BaseConverter.h"
#include "PixelCodec.h"

#include <iostream>
#include <string>
#include <string_view>

namespace {

class TestSuite final {
public:
    void check(const bool condition, const std::string_view name) {
        if (condition) {
            std::cout << "[PASS] " << name << '\n';
            return;
        }
        std::cerr << "[FAIL] " << name << '\n';
        ++failures_;
    }

    [[nodiscard]] int result() const {
        return failures_ == 0 ? 0 : 1;
    }

private:
    int failures_{0};
};

void testLatin1(TestSuite& suite) {
    const auto letter = AsciiConverter::toDecimal("A");
    suite.check(letter.success && letter.decimalValues == "65", "Latin-1 letter");

    const auto phrase = AsciiConverter::toDecimal("Az 0");
    suite.check(
        phrase.success && phrase.decimalValues == "65 122 32 48",
        "Latin-1 phrase");

    const std::string boundaries{
        static_cast<char>(0),
        static_cast<char>(127),
        static_cast<char>(128),
        static_cast<char>(255)
    };
    const auto boundaryResult = AsciiConverter::toDecimal(boundaries);
    suite.check(
        boundaryResult.success
            && boundaryResult.decimalValues == "0 127 128 255",
        "Latin-1 accepts values 0 through 255");

    suite.check(
        !AsciiConverter::toDecimal(U"\u0100").success,
        "Latin-1 rejects Unicode values above 255");
    suite.check(!AsciiConverter::toDecimal("").success, "Latin-1 rejects empty input");
}

void testBaseConverter(TestSuite& suite) {
    using Base = BaseConverter::Base;
    using Representation = BaseConverter::Representation;

    const auto hexadecimal = BaseConverter::convert(
        "42 255", Base::decimal, Base::hexadecimal);
    suite.check(
        hexadecimal.success
            && hexadecimal.values.size() == 2U
            && hexadecimal.values[0].output == "2A"
            && hexadecimal.values[1].output == "FF",
        "Base converter handles multiple values");

    const auto negative = BaseConverter::convert(
        "11111111",
        Base::binary,
        Base::decimal,
        Representation::twosComplement,
        8U);
    suite.check(
        negative.success && negative.values[0].output == "-1",
        "Base converter handles two's complement");

    suite.check(
        !BaseConverter::convert("102", Base::binary, Base::decimal).success,
        "Base converter rejects invalid digits");
}

void testPixelCodec(TestSuite& suite) {
    const PixelImage image{
        2U,
        1U,
        {
            {255U, 0U, 16U, 255U},
            {1U, 2U, 3U, 4U}
        }
    };

    const auto encoded = PixelCodec::encode(image);
    suite.check(
        encoded.success
            && encoded.text.find("#FF0010FF #01020304") != std::string::npos,
        "Pixel codec encodes RGBA pixels");

    const auto decoded = PixelCodec::decode(encoded.text);
    suite.check(
        decoded.success && decoded.image == image,
        "Pixel codec round trip");

    suite.check(
        !PixelCodec::decode(
            "IAB-IMAGE 1\nWIDTH 1\nHEIGHT 1\nFORMAT RGBA8\nPIXELS\n#GG0000FF\n")
             .success,
        "Pixel codec rejects invalid hexadecimal");
}

} // namespace

int main() {
    TestSuite suite;
    testLatin1(suite);
    testBaseConverter(suite);
    testPixelCodec(suite);
    return suite.result();
}
