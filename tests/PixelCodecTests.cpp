#include "PixelCodec.h"

#include <cassert>
#include <string>

int main() {
    const PixelImage source{
        2U,
        1U,
        {
            {255U, 0U, 16U, 255U},
            {1U, 2U, 3U, 4U}
        }
    };

    const auto encoded = PixelCodec::encode(source);
    assert(encoded.success);
    assert(encoded.text.find("#FF0010FF #01020304") != std::string::npos);

    const auto decoded = PixelCodec::decode(encoded.text);
    assert(decoded.success);
    assert(decoded.image == source);

    assert(!PixelCodec::decode("").success);
    assert(!PixelCodec::decode(
        "IAB-IMAGE 1\nWIDTH 1\nHEIGHT 1\nFORMAT RGBA8\nPIXELS\n#GG0000FF\n")
        .success);
}
