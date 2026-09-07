#ifndef BASE_CONVERTER_H
#define BASE_CONVERTER_H

#include <string>
#include <string_view>
#include <vector>

class BaseConverter final {
public:
    enum class Base : unsigned int {
        binary = 2U,
        octal = 8U,
        decimal = 10U,
        hexadecimal = 16U
    };

    enum class Representation {
        unsignedInteger,
        twosComplement
    };

    struct ConvertedValue final {
        std::string input;
        std::string output;
    };

    struct Result final {
        bool success{false};
        std::vector<ConvertedValue> values;
        std::string errorMessage;
    };

    [[nodiscard]] static Result convert(
        std::string_view text,
        Base inputBase,
        Base outputBase,
        Representation representation = Representation::unsignedInteger,
        unsigned int bitWidth = 8U);
};

#endif
