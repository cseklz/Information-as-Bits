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
        ++checkCount_;
        if (!condition) {
            ++failureCount_;
            std::cerr << "FAIL: " << name << '\n';
        }
    }

    void checkEqual(
        const std::string_view actual,
        const std::string_view expected,
        const std::string_view name) {
        const bool equal = actual == expected;
        check(equal, name);
        if (!equal) {
            std::cerr << "  expected: " << expected << "\n  actual:   " << actual << '\n';
        }
    }

    [[nodiscard]] int finish() const {
        const int passed = checkCount_ - failureCount_;
        std::cout << passed << '/' << checkCount_ << " checks passed.\n";
        return failureCount_ == 0 ? 0 : 1;
    }

private:
    int checkCount_{0};
    int failureCount_{0};
};

[[nodiscard]] std::string firstOutput(const BaseConverter::Result& result) {
    return (result.success && !result.values.empty()) ? result.values.front().output : "";
}

void testAscii(TestSuite& suite) {
    const AsciiConverter::Result letter = AsciiConverter::toDecimal("A");
    suite.check(letter.success, "ASCII accepts A");
    suite.checkEqual(letter.decimalValues, "65", "ASCII A is decimal 65");

    const AsciiConverter::Result phrase = AsciiConverter::toDecimal("Az 0");
    suite.checkEqual(
        phrase.decimalValues,
        "65 122 32 48",
        "ASCII converts several characters in order");

    const std::string nul(1U, '\0');
    suite.checkEqual(
        AsciiConverter::toDecimal(nul).decimalValues,
        "0",
        "ASCII lower boundary NUL is zero");

    const std::string del(1U, static_cast<char>(127));
    suite.checkEqual(
        AsciiConverter::toDecimal(del).decimalValues,
        "127",
        "ASCII upper boundary DEL is 127");

    const std::string nonAscii(1U, static_cast<char>(128));
    suite.check(!AsciiConverter::toDecimal(nonAscii).success, "ASCII rejects byte 128");
    suite.check(!AsciiConverter::toDecimal("").success, "ASCII rejects empty input");
}

void testUnsignedBases(TestSuite& suite) {
    using Base = BaseConverter::Base;

    const auto binary = BaseConverter::convert("101010", Base::binary, Base::decimal);
    suite.checkEqual(firstOutput(binary), "42", "Binary converts to decimal");

    const auto octal = BaseConverter::convert("52", Base::octal, Base::decimal);
    suite.checkEqual(firstOutput(octal), "42", "Octal converts to decimal");

    const auto hexadecimal = BaseConverter::convert("2a", Base::hexadecimal, Base::decimal);
    suite.checkEqual(firstOutput(hexadecimal), "42", "Lowercase hexadecimal is accepted");

    const auto uppercase = BaseConverter::convert("42", Base::decimal, Base::hexadecimal);
    suite.checkEqual(firstOutput(uppercase), "2A", "Hexadecimal output is uppercase");

    const auto leadingZeros = BaseConverter::convert("00042", Base::decimal, Base::decimal);
    suite.checkEqual(firstOutput(leadingZeros), "42", "Leading zeros are normalized");

    const auto zero = BaseConverter::convert("0", Base::decimal, Base::binary);
    suite.checkEqual(firstOutput(zero), "0", "Unsigned zero is handled explicitly");

    const auto maximumHex = BaseConverter::convert(
        "18446744073709551615",
        Base::decimal,
        Base::hexadecimal);
    suite.checkEqual(
        firstOutput(maximumHex),
        "FFFFFFFFFFFFFFFF",
        "UINT64_MAX converts to hexadecimal");

    const auto maximumBinary = BaseConverter::convert(
        "18446744073709551615",
        Base::decimal,
        Base::binary);
    suite.checkEqual(
        firstOutput(maximumBinary),
        std::string(64U, '1'),
        "UINT64_MAX converts to 64 binary ones");

    const auto maximumOctal = BaseConverter::convert(
        "18446744073709551615",
        Base::decimal,
        Base::octal);
    suite.checkEqual(
        firstOutput(maximumOctal),
        "1777777777777777777777",
        "UINT64_MAX converts to octal");

    const auto batch = BaseConverter::convert("10101 111", Base::binary, Base::decimal);
    suite.check(batch.success && (batch.values.size() == 2U), "Batch conversion returns two values");
    if (batch.success && (batch.values.size() == 2U)) {
        suite.checkEqual(batch.values[0].output, "21", "First batch value converts");
        suite.checkEqual(batch.values[1].output, "7", "Second batch value converts");
    }

    suite.check(
        !BaseConverter::convert("18446744073709551616", Base::decimal, Base::hexadecimal)
             .success,
        "Unsigned overflow is rejected");
    suite.check(!BaseConverter::convert("102", Base::binary, Base::decimal).success,
                "Invalid binary digit is rejected");
    suite.check(!BaseConverter::convert("8", Base::octal, Base::decimal).success,
                "Invalid octal digit is rejected");
    suite.check(!BaseConverter::convert("12x", Base::decimal, Base::binary).success,
                "Invalid decimal digit is rejected");
    suite.check(!BaseConverter::convert("G1", Base::hexadecimal, Base::decimal).success,
                "Invalid hexadecimal digit is rejected");
    suite.check(!BaseConverter::convert("0x2A", Base::hexadecimal, Base::decimal).success,
                "Prefixes are rejected consistently");
    suite.check(!BaseConverter::convert("-1", Base::decimal, Base::binary).success,
                "Unsigned mode rejects a negative value");
    suite.check(!BaseConverter::convert("   ", Base::decimal, Base::binary).success,
                "Whitespace-only input is rejected");
}

void testTwosComplement(TestSuite& suite) {
    using Base = BaseConverter::Base;
    using Representation = BaseConverter::Representation;
    constexpr auto signedMode = Representation::twosComplement;

    suite.checkEqual(
        firstOutput(BaseConverter::convert("-1", Base::decimal, Base::binary, signedMode, 8U)),
        "11111111",
        "Negative one encodes as 8-bit two's complement");
    suite.checkEqual(
        firstOutput(BaseConverter::convert("-1", Base::decimal, Base::octal, signedMode, 8U)),
        "377",
        "Negative one encodes as 8-bit octal");
    suite.checkEqual(
        firstOutput(BaseConverter::convert("-128", Base::decimal, Base::hexadecimal, signedMode, 8U)),
        "80",
        "Minimum signed 8-bit value encodes correctly");
    suite.checkEqual(
        firstOutput(BaseConverter::convert("11111111", Base::binary, Base::decimal, signedMode, 8U)),
        "-1",
        "8-bit pattern decodes as negative one");
    suite.checkEqual(
        firstOutput(BaseConverter::convert("0", Base::decimal, Base::binary, signedMode, 8U)),
        "00000000",
        "Signed zero uses the selected word width");
    suite.checkEqual(
        firstOutput(BaseConverter::convert(
            "-32768", Base::decimal, Base::hexadecimal, signedMode, 16U)),
        "8000",
        "Minimum signed 16-bit value encodes correctly");
    suite.checkEqual(
        firstOutput(BaseConverter::convert(
            "FFFFFFFF", Base::hexadecimal, Base::decimal, signedMode, 32U)),
        "-1",
        "32-bit all-ones pattern decodes as negative one");
    suite.checkEqual(
        firstOutput(BaseConverter::convert(
            "-9223372036854775808",
            Base::decimal,
            Base::hexadecimal,
            signedMode,
            64U)),
        "8000000000000000",
        "Minimum signed 64-bit value encodes correctly");
    suite.checkEqual(
        firstOutput(BaseConverter::convert(
            "FFFFFFFFFFFFFFFF",
            Base::hexadecimal,
            Base::decimal,
            signedMode,
            64U)),
        "-1",
        "64-bit all-ones pattern decodes as negative one");

    suite.check(
        !BaseConverter::convert("-129", Base::decimal, Base::binary, signedMode, 8U).success,
        "Value below signed 8-bit minimum is rejected");
    suite.check(
        !BaseConverter::convert("128", Base::decimal, Base::binary, signedMode, 8U).success,
        "Value above signed 8-bit maximum is rejected");
    suite.check(
        !BaseConverter::convert("100000000", Base::binary, Base::decimal, signedMode, 8U).success,
        "Bit pattern wider than selected width is rejected");
    suite.check(
        !BaseConverter::convert("-1", Base::decimal, Base::binary, signedMode, 7U).success,
        "Unsupported word width is rejected");
    suite.check(
        !BaseConverter::convert(
             "1",
             Base::decimal,
             Base::binary,
             static_cast<Representation>(99),
             0U)
             .success,
        "Unsupported number interpretation is rejected");
}

void testPixelCodec(TestSuite& suite) {
    const PixelImage image{
        2U,
        2U,
        {
            {0xEDU, 0x6AU, 0x5AU, 0xFFU},
            {0x80U, 0x8FU, 0x85U, 0x80U},
            {0x59U, 0x59U, 0x59U, 0x00U},
            {0xEBU, 0xF8U, 0xB8U, 0xFFU}
        }
    };
    const std::string expected =
        "IAB-IMAGE 1\n"
        "WIDTH 2\n"
        "HEIGHT 2\n"
        "FORMAT RGBA8\n"
        "PIXELS\n"
        "#ED6A5AFF #808F8580\n"
        "#59595900 #EBF8B8FF\n";

    const PixelCodec::EncodeResult encoded = PixelCodec::encode(image);
    suite.check(encoded.success, "Pixel image encodes successfully");
    suite.checkEqual(encoded.text, expected, "Pixel text has a deterministic format");

    const PixelCodec::DecodeResult decoded = PixelCodec::decode(expected);
    suite.check(decoded.success, "Valid pixel text decodes successfully");
    suite.check(decoded.image == image, "Pixel text round trip preserves RGBA values");

    const PixelCodec::DecodeResult lowercase = PixelCodec::decode(
        "IAB-IMAGE 1 WIDTH 1 HEIGHT 1 FORMAT RGBA8 PIXELS #ed6a5a80");
    suite.check(lowercase.success, "Lowercase pixel hexadecimal is accepted");
    if (lowercase.success) {
        suite.checkEqual(
            PixelCodec::encode(lowercase.image).text,
            "IAB-IMAGE 1\nWIDTH 1\nHEIGHT 1\nFORMAT RGBA8\nPIXELS\n#ED6A5A80\n",
            "Pixel export canonicalizes hexadecimal to uppercase");
    }

    suite.check(!PixelCodec::decode("").success, "Empty pixel text is rejected");
    suite.check(
        !PixelCodec::decode("NOT-IAB 1 WIDTH 1 HEIGHT 1 FORMAT RGBA8 PIXELS #000000FF").success,
        "Bad pixel magic is rejected");
    suite.check(
        !PixelCodec::decode("IAB-IMAGE 2 WIDTH 1 HEIGHT 1 FORMAT RGBA8 PIXELS #000000FF").success,
        "Unknown pixel version is rejected");
    suite.check(
        !PixelCodec::decode("IAB-IMAGE 1 WIDTH 0 HEIGHT 1 FORMAT RGBA8 PIXELS").success,
        "Zero width is rejected");
    suite.check(
        !PixelCodec::decode("IAB-IMAGE 1 WIDTH -1 HEIGHT 1 FORMAT RGBA8 PIXELS").success,
        "Negative width is rejected");
    suite.check(
        !PixelCodec::decode("IAB-IMAGE 1 WIDTH 1 HEIGHT 1 FORMAT RGB8 PIXELS #000000FF").success,
        "Unsupported pixel format is rejected");
    suite.check(
        !PixelCodec::decode("IAB-IMAGE 1 WIDTH 1 HEIGHT 1 FORMAT RGBA8 PIXELS #GG0000FF").success,
        "Invalid pixel hexadecimal is rejected");
    suite.check(
        !PixelCodec::decode("IAB-IMAGE 1 WIDTH 2 HEIGHT 1 FORMAT RGBA8 PIXELS #000000FF").success,
        "Missing pixel is rejected");
    suite.check(
        !PixelCodec::decode(
             "IAB-IMAGE 1 WIDTH 1 HEIGHT 1 FORMAT RGBA8 PIXELS #000000FF #FFFFFFFF")
             .success,
        "Extra pixel is rejected");
    suite.check(
        !PixelCodec::decode(
             "IAB-IMAGE 1 WIDTH 1000001 HEIGHT 1 FORMAT RGBA8 PIXELS")
             .success,
        "Oversized pixel image is rejected");

    const PixelImage invalidDimensions{0U, 1U, {}};
    suite.check(!PixelCodec::encode(invalidDimensions).success,
                "Encoder rejects zero dimensions");
    const PixelImage wrongCount{2U, 1U, {{0U, 0U, 0U, 255U}}};
    suite.check(!PixelCodec::encode(wrongCount).success,
                "Encoder rejects a mismatched pixel count");
}

} // namespace

int main() {
    TestSuite suite;
    testAscii(suite);
    testUnsignedBases(suite);
    testTwosComplement(suite);
    testPixelCodec(suite);
    return suite.finish();
}
