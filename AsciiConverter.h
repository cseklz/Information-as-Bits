#pragma once

#include <string>
#include <string_view>

class AsciiConverter final {
public:
    struct Result final {
        bool success{false};
        std::string decimalValues;
        std::string errorMessage;
    };

    // This project uses "ASCII" to mean the ISO-8859-1/Latin-1 range.
    [[nodiscard]] static Result toDecimal(std::u32string_view text);
};
