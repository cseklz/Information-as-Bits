#include "AsciiConverter.h"

#include <cstddef>
#include <string>
#include <utility>

namespace {
constexpr char32_t latin1Maximum = 255;
}

AsciiConverter::Result AsciiConverter::toDecimal(const std::string_view text) {
    if (text.empty()) {
        return {false, {}, "Enter at least one Latin-1 character."};
    }

    std::string decimalValues;
    decimalValues.reserve(text.size() * 4U);

    for (const char character : text) {
        if (!decimalValues.empty()) {
            decimalValues.push_back(' ');
        }
        const auto value = static_cast<unsigned char>(character);
        decimalValues += std::to_string(static_cast<unsigned int>(value));
    }

    return {true, std::move(decimalValues), {}};
}

AsciiConverter::Result AsciiConverter::toDecimal(const std::u32string_view text) {
    if (text.empty()) {
        return {false, {}, "Enter at least one Latin-1 character."};
    }

    std::string decimalValues;
    decimalValues.reserve(text.size() * 4U);

    for (std::size_t index = 0U; index < text.size(); ++index) {
        const char32_t value = text[index];
        if (value > latin1Maximum) {
            return {
                false,
                {},
                "Character " + std::to_string(index + 1U) +
                    " is outside the Latin-1 range (0-255)."
            };
        }

        if (!decimalValues.empty()) {
            decimalValues.push_back(' ');
        }
        decimalValues += std::to_string(static_cast<unsigned int>(value));
    }

    return {true, std::move(decimalValues), {}};
}
