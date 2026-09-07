#include "PixelCodec.h"

#include <cstddef>
#include <cstdint>
#include <limits>
#include <sstream>
#include <string>
#include <utility>

namespace {

constexpr std::string_view kHexDigits{"0123456789ABCDEF"};
[[nodiscard]] PixelCodec::DecodeResult decodeError(std::string message) {
    return {false, {}, std::move(message)};
}

[[nodiscard]] bool parseDecimal(
    const std::string_view token,
    std::uint32_t& value) {
    if (token.empty()) {
        return false;
    }

    std::uint64_t parsed{0U};
    for (const char character : token) {
        if ((character < '0') || (character > '9')) {
            return false;
        }
        parsed = (parsed * 10U) + static_cast<std::uint64_t>(character - '0');
        if (parsed > std::numeric_limits<std::uint32_t>::max()) {
            return false;
        }
    }
    value = static_cast<std::uint32_t>(parsed);
    return true;
}

[[nodiscard]] int hexValue(const char character) {
    if ((character >= '0') && (character <= '9')) {
        return character - '0';
    }
    if ((character >= 'A') && (character <= 'F')) {
        return 10 + (character - 'A');
    }
    if ((character >= 'a') && (character <= 'f')) {
        return 10 + (character - 'a');
    }
    return -1;
}

[[nodiscard]] bool parseByte(
    const std::string_view text,
    const std::size_t offset,
    std::uint8_t& value) {
    const int high = hexValue(text[offset]);
    const int low = hexValue(text[offset + 1U]);
    if ((high < 0) || (low < 0)) {
        return false;
    }
    value = static_cast<std::uint8_t>((high * 16) + low);
    return true;
}

[[nodiscard]] bool parsePixel(const std::string_view token, Pixel& pixel) {
    if ((token.size() != 9U) || (token.front() != '#')) {
        return false;
    }
    return parseByte(token, 1U, pixel.red)
        && parseByte(token, 3U, pixel.green)
        && parseByte(token, 5U, pixel.blue)
        && parseByte(token, 7U, pixel.alpha);
}

void appendByte(std::string& output, const std::uint8_t value) {
    output.push_back(kHexDigits[value >> 4U]);
    output.push_back(kHexDigits[value & 0x0FU]);
}

void appendPixel(std::string& output, const Pixel& pixel) {
    output.push_back('#');
    appendByte(output, pixel.red);
    appendByte(output, pixel.green);
    appendByte(output, pixel.blue);
    appendByte(output, pixel.alpha);
}

} // namespace

PixelCodec::EncodeResult PixelCodec::encode(const PixelImage& image) {
    if ((image.width == 0U) || (image.height == 0U)) {
        return {false, {}, "Image width and height must both be positive."};
    }

    const std::uint64_t pixelCount = static_cast<std::uint64_t>(image.width)
        * static_cast<std::uint64_t>(image.height);
    if (pixelCount > maximumPixelCount) {
        return {false, {}, "The image exceeds the 1,000,000-pixel limit."};
    }
    if (image.pixels.size() != pixelCount) {
        return {false, {}, "The pixel count does not match the image dimensions."};
    }

    std::string output;
    output.reserve(static_cast<std::size_t>((pixelCount * 10U) + 80U));
    output += "IAB-IMAGE 1\nWIDTH ";
    output += std::to_string(image.width);
    output += "\nHEIGHT ";
    output += std::to_string(image.height);
    output += "\nFORMAT RGBA8\nPIXELS\n";

    for (std::uint32_t row = 0U; row < image.height; ++row) {
        for (std::uint32_t column = 0U; column < image.width; ++column) {
            if (column != 0U) {
                output.push_back(' ');
            }
            const std::uint64_t index = (static_cast<std::uint64_t>(row) * image.width)
                + column;
            appendPixel(output, image.pixels[static_cast<std::size_t>(index)]);
        }
        output.push_back('\n');
    }

    return {true, std::move(output), {}};
}

PixelCodec::DecodeResult PixelCodec::decode(const std::string_view text) {
    if (text.empty()) {
        return decodeError("The pixel text is empty.");
    }
    if (text.size() > maximumTextBytes) {
        return decodeError("The pixel text exceeds the 12 MB safety limit.");
    }

    std::istringstream input{std::string(text)};
    std::string token;

    if (!(input >> token) || (token != "IAB-IMAGE")) {
        return decodeError("Expected the IAB-IMAGE file header.");
    }
    if (!(input >> token) || (token != "1")) {
        return decodeError("Only IAB-IMAGE version 1 is supported.");
    }
    if (!(input >> token) || (token != "WIDTH")) {
        return decodeError("Expected the WIDTH header.");
    }

    std::string widthText;
    std::uint32_t width{0U};
    if (!(input >> widthText) || !parseDecimal(widthText, width) || (width == 0U)) {
        return decodeError("WIDTH must be a positive whole number.");
    }
    if (!(input >> token) || (token != "HEIGHT")) {
        return decodeError("Expected the HEIGHT header.");
    }

    std::string heightText;
    std::uint32_t height{0U};
    if (!(input >> heightText) || !parseDecimal(heightText, height) || (height == 0U)) {
        return decodeError("HEIGHT must be a positive whole number.");
    }
    if (!(input >> token) || (token != "FORMAT")) {
        return decodeError("Expected the FORMAT header.");
    }
    if (!(input >> token) || (token != "RGBA8")) {
        return decodeError("Only the RGBA8 pixel format is supported.");
    }
    if (!(input >> token) || (token != "PIXELS")) {
        return decodeError("Expected the PIXELS header.");
    }

    const std::uint64_t pixelCount = static_cast<std::uint64_t>(width)
        * static_cast<std::uint64_t>(height);
    if (pixelCount > maximumPixelCount) {
        return decodeError("The image exceeds the 1,000,000-pixel limit.");
    }

    PixelImage image;
    image.width = width;
    image.height = height;
    image.pixels.reserve(static_cast<std::size_t>(pixelCount));

    for (std::uint64_t index = 0U; index < pixelCount; ++index) {
        if (!(input >> token)) {
            return decodeError(
                "Expected " + std::to_string(pixelCount) + " pixels, but found "
                    + std::to_string(index) + ".");
        }

        Pixel pixel;
        if (!parsePixel(token, pixel)) {
            return decodeError(
                "Pixel " + std::to_string(index + 1U)
                    + " must use #RRGGBBAA hexadecimal notation.");
        }
        image.pixels.push_back(pixel);
    }

    if (input >> token) {
        return decodeError(
            "The file contains more than the expected " + std::to_string(pixelCount)
                + " pixels.");
    }

    return {true, std::move(image), {}};
}
