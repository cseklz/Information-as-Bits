#include "AsciiConverter.h"

#include <cassert>
#include <string>

int main() {
    {
        const auto result = AsciiConverter::toDecimal("Az 0");
        assert(result.success);
        assert(result.decimalValues == "65 122 32 48");
    }
    {
        const std::string byteValues{
            static_cast<char>(0),
            static_cast<char>(127),
            static_cast<char>(128),
            static_cast<char>(255)
        };
        const auto result = AsciiConverter::toDecimal(byteValues);
        assert(result.success);
        assert(result.decimalValues == "0 127 128 255");
    }
    {
        const auto result = AsciiConverter::toDecimal(U"Hello!");
        assert(result.success);
        assert(result.decimalValues == "72 101 108 108 111 33");
    }
    {
        const std::u32string boundaryValues{0, 127, 128, 255};
        const auto result = AsciiConverter::toDecimal(boundaryValues);
        assert(result.success);
        assert(result.decimalValues == "0 127 128 255");
    }
    {
        const auto result = AsciiConverter::toDecimal(U"caf\u00E9");
        assert(result.success);
        assert(result.decimalValues == "99 97 102 233");
    }
    {
        const auto result = AsciiConverter::toDecimal(U"A\u0100");
        assert(!result.success);
        assert(result.errorMessage.find("Character 2") != std::string::npos);
    }
    {
        const auto result = AsciiConverter::toDecimal(U"");
        assert(!result.success);
    }
}
