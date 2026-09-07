#include "AsciiConverter.h"

#include <cstddef>
#include <string>
#include <utility>

namespace {
constexpr char32_t latin1Maximum = 255;
} // namespace

AsciiConverter::Result AsciiConverter::toDecimal(const std::string_view text) {
    if (text.empty()) {
        return {.success = false, .decimalValues = {}, .errorMessage = "Enter at least one ASCII character."};
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

    return {.success = true, .decimalValues = std::move(decimalValues), .errorMessage = {}};
}

AsciiConverter::Result AsciiConverter::toDecimal(const std::u32string_view text) {
    if (text.empty()) {
        return {.success = false, .decimalValues = {}, .errorMessage = "Enter at least one ASCII character."};
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
                    " is outside the ASCII range (0-255)."
            };
        }

        if (!decimalValues.empty()) {
            decimalValues.push_back(' ');
        }
        decimalValues += std::to_string(value);
    }

    return {.success = true, .decimalValues = std::move(decimalValues), .errorMessage = {}};
}