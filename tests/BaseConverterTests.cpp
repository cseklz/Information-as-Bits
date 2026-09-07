#include "BaseConverter.h"

#include <cassert>

int main() {
    using Base = BaseConverter::Base;
    using Representation = BaseConverter::Representation;

    {
        const auto result = BaseConverter::convert(
            "42 255", Base::decimal, Base::hexadecimal);
        assert(result.success);
        assert(result.values.size() == 2U);
        assert(result.values[0].output == "2A");
        assert(result.values[1].output == "FF");
    }
    {
        const auto result = BaseConverter::convert(
            "11111111",
            Base::binary,
            Base::decimal,
            Representation::twosComplement,
            8U);
        assert(result.success);
        assert(result.values[0].output == "-1");
    }
    {
        const auto result = BaseConverter::convert(
            "-128",
            Base::decimal,
            Base::hexadecimal,
            Representation::twosComplement,
            8U);
        assert(result.success);
        assert(result.values[0].output == "80");
    }
    {
        const auto result = BaseConverter::convert(
            "102", Base::binary, Base::decimal);
        assert(!result.success);
    }
}
