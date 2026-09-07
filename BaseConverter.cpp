#include "BaseConverter.h"

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <limits>
#include <sstream>
#include <string>
#include <utility>

namespace {

constexpr std::string_view kDigits{"0123456789ABCDEF"};
constexpr std::size_t kMaximumTokenCount{1024U};

struct ParsedNumber final {
    bool success{false};
    std::uint64_t value{0U};
    std::string errorMessage;
};

[[nodiscard]] bool isSupportedBase(const BaseConverter::Base base) {
    switch (base) {
        case BaseConverter::Base::binary:
        case BaseConverter::Base::octal:
        case BaseConverter::Base::decimal:
        case BaseConverter::Base::hexadecimal:
            return true;
    }
    return false;
}

[[nodiscard]] unsigned int baseValue(const BaseConverter::Base base) {
    return static_cast<unsigned int>(base);
}

[[nodiscard]] int digitValue(const char character) {
    const auto byte = static_cast<unsigned char>(character);
    if (std::isdigit(byte) != 0) {
        return character - '0';
    }

    const char upper = static_cast<char>(std::toupper(byte));
    if ((upper >= 'A') && (upper <= 'F')) {
        return 10 + (upper - 'A');
    }
    return -1;
}

[[nodiscard]] ParsedNumber parseUnsigned(
    const std::string_view token,
    const BaseConverter::Base base) {
    if (token.empty()) {
        return {false, 0U, "A number cannot be empty."};
    }
    if ((token.front() == '+') || (token.front() == '-')) {
        return {false, 0U, "Signs are not valid in unsigned mode."};
    }

    const std::uint64_t maximum = std::numeric_limits<std::uint64_t>::max();
    const auto radix = static_cast<std::uint64_t>(baseValue(base));
    const std::uint64_t maximumQuotient = maximum / radix;
    const std::uint64_t maximumRemainder = maximum % radix;
    std::uint64_t value{0U};

    for (const char character : token) {
        const int digit = digitValue(character);
        if ((digit < 0) || (static_cast<unsigned int>(digit) >= baseValue(base))) {
            return {
                false,
                0U,
                "Digit '" + std::string(1U, character)
                    + "' is not valid for base " + std::to_string(baseValue(base)) + "."
            };
        }

        const auto unsignedDigit = static_cast<std::uint64_t>(digit);
        if ((value > maximumQuotient)
            || ((value == maximumQuotient) && (unsignedDigit > maximumRemainder))) {
            return {false, 0U, "The value is larger than UINT64_MAX."};
        }
        value = (value * radix) + unsignedDigit;
    }

    return {true, value, {}};
}

[[nodiscard]] std::uint64_t maskForWidth(const unsigned int bitWidth) {
    if (bitWidth == 64U) {
        return std::numeric_limits<std::uint64_t>::max();
    }
    return (std::uint64_t{1U} << bitWidth) - 1U;
}

[[nodiscard]] bool isSupportedWidth(const unsigned int bitWidth) {
    return (bitWidth == 8U) || (bitWidth == 16U)
        || (bitWidth == 32U) || (bitWidth == 64U);
}

[[nodiscard]] ParsedNumber parseTwosComplement(
    const std::string_view token,
    const BaseConverter::Base inputBase,
    const unsigned int bitWidth) {
    const std::uint64_t mask = maskForWidth(bitWidth);
    const std::uint64_t negativeLimit = std::uint64_t{1U} << (bitWidth - 1U);
    const std::uint64_t positiveLimit = negativeLimit - 1U;

    if (inputBase != BaseConverter::Base::decimal) {
        const ParsedNumber parsed = parseUnsigned(token, inputBase);
        if (!parsed.success) {
            return parsed;
        }
        if (parsed.value > mask) {
            return {
                false,
                0U,
                "The bit pattern does not fit in " + std::to_string(bitWidth) + " bits."
            };
        }
        return parsed;
    }

    const bool isNegative = !token.empty() && (token.front() == '-');
    const std::string_view magnitudeText = isNegative ? token.substr(1U) : token;
    if (magnitudeText.empty()) {
        return {false, 0U, "A minus sign must be followed by decimal digits."};
    }
    if (!isNegative && (token.front() == '+')) {
        return {false, 0U, "A leading plus sign is not supported."};
    }

    const ParsedNumber magnitude = parseUnsigned(
        magnitudeText,
        BaseConverter::Base::decimal);
    if (!magnitude.success) {
        return magnitude;
    }

    if (isNegative) {
        if (magnitude.value > negativeLimit) {
            return {
                false,
                0U,
                "The value is below the signed " + std::to_string(bitWidth)
                    + "-bit minimum."
            };
        }
        return {true, (std::uint64_t{0U} - magnitude.value) & mask, {}};
    }

    if (magnitude.value > positiveLimit) {
        return {
            false,
            0U,
            "The value is above the signed " + std::to_string(bitWidth)
                + "-bit maximum."
        };
    }
    return {true, magnitude.value, {}};
}

[[nodiscard]] std::string formatUnsigned(
    std::uint64_t value,
    const BaseConverter::Base base) {
    if (value == 0U) {
        return "0";
    }

    const auto radix = static_cast<std::uint64_t>(baseValue(base));
    std::string output;
    output.reserve(64U);

    while (value > 0U) {
        const auto remainder = static_cast<std::size_t>(value % radix);
        output.push_back(kDigits[remainder]);
        value /= radix;
    }

    std::reverse(output.begin(), output.end());
    return output;
}

[[nodiscard]] std::size_t encodedWidth(
    const BaseConverter::Base base,
    const unsigned int bitWidth) {
    switch (base) {
        case BaseConverter::Base::binary:
            return bitWidth;
        case BaseConverter::Base::octal:
            return (bitWidth + 2U) / 3U;
        case BaseConverter::Base::hexadecimal:
            return (bitWidth + 3U) / 4U;
        case BaseConverter::Base::decimal:
            return 1U;
    }
    return 1U;
}

[[nodiscard]] std::string formatTwosComplement(
    const std::uint64_t value,
    const BaseConverter::Base outputBase,
    const unsigned int bitWidth) {
    if (outputBase == BaseConverter::Base::decimal) {
        const std::uint64_t signBit = std::uint64_t{1U} << (bitWidth - 1U);
        if ((value & signBit) != 0U) {
            const std::uint64_t magnitude = ((~value) & maskForWidth(bitWidth)) + 1U;
            return "-" + formatUnsigned(magnitude, BaseConverter::Base::decimal);
        }
        return formatUnsigned(value, BaseConverter::Base::decimal);
    }

    std::string output = formatUnsigned(value, outputBase);
    const std::size_t minimumWidth = encodedWidth(outputBase, bitWidth);
    if (output.size() < minimumWidth) {
        output.insert(output.begin(), minimumWidth - output.size(), '0');
    }
    return output;
}

} // namespace

BaseConverter::Result BaseConverter::convert(
    const std::string_view text,
    const Base inputBase,
    const Base outputBase,
    const Representation representation,
    const unsigned int bitWidth) {
    if (!isSupportedBase(inputBase) || !isSupportedBase(outputBase)) {
        return {false, {}, "Only bases 2, 8, 10, and 16 are supported."};
    }
    if ((representation != Representation::unsignedInteger)
        && (representation != Representation::twosComplement)) {
        return {false, {}, "The selected number interpretation is not supported."};
    }
    if ((representation == Representation::twosComplement)
        && !isSupportedWidth(bitWidth)) {
        return {false, {}, "Two's-complement width must be 8, 16, 32, or 64 bits."};
    }

    std::istringstream inputStream{std::string(text)};
    std::vector<ConvertedValue> convertedValues;
    std::string token;

    while (inputStream >> token) {
        if (convertedValues.size() >= kMaximumTokenCount) {
            return {false, {}, "Convert at most 1024 values at one time."};
        }

        const ParsedNumber parsed = representation == Representation::unsignedInteger
            ? parseUnsigned(token, inputBase)
            : parseTwosComplement(token, inputBase, bitWidth);

        if (!parsed.success) {
            return {
                false,
                {},
                "Value " + std::to_string(convertedValues.size() + 1U)
                    + " (\"" + token + "\"): " + parsed.errorMessage
            };
        }

        const std::string output = representation == Representation::unsignedInteger
            ? formatUnsigned(parsed.value, outputBase)
            : formatTwosComplement(parsed.value, outputBase, bitWidth);
        convertedValues.push_back({std::move(token), output});
    }

    if (convertedValues.empty()) {
        return {false, {}, "Enter at least one value to convert."};
    }
    return {true, std::move(convertedValues), {}};
}
