#ifndef PIXEL_CODEC_H
#define PIXEL_CODEC_H

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

struct Pixel final {
    std::uint8_t red{0U};
    std::uint8_t green{0U};
    std::uint8_t blue{0U};
    std::uint8_t alpha{255U};

    [[nodiscard]] bool operator==(const Pixel&) const = default;
};

struct PixelImage final {
    std::uint32_t width{0U};
    std::uint32_t height{0U};
    std::vector<Pixel> pixels;

    [[nodiscard]] bool operator==(const PixelImage&) const = default;
};

class PixelCodec final {
public:
    static constexpr std::uint64_t maximumPixelCount{1'000'000U};
    static constexpr std::uint64_t maximumTextBytes{12'000'000U};

    struct EncodeResult final {
        bool success{false};
        std::string text;
        std::string errorMessage;
    };

    struct DecodeResult final {
        bool success{false};
        PixelImage image;
        std::string errorMessage;
    };

    [[nodiscard]] static EncodeResult encode(const PixelImage& image);
    [[nodiscard]] static DecodeResult decode(std::string_view text);
};

#endif
