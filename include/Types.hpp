#ifndef DITHER_TYPES_HPP
#define DITHER_TYPES_HPP

namespace dither
{
    enum MAP_TYPE
    {
        bayer_2x2,
        bayer_4x4,
        bayer_8x8,
        clustered_3x3_1,
        clustered_3x3_2
    };

    enum PATTERN_TYPE
    {
        clustered,
        dispersed
    };
}

#endif //DITHER_TYPES_HPP
