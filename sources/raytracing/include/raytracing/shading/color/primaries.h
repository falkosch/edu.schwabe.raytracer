#pragma once

#include <vectorization.h>

namespace raytracer::color
{
    using namespace vectorization;

    // ITU-R BT.709 / IEC 61966-2-1 (sRGB), D65 white point
    const MFloat4x4& sRGBLinearToXYZ() noexcept;
    const MFloat4x4& XYZToSRGBLinear() noexcept;

    // ITU-R BT.2020 (Rec.2020), D65 white point
    const MFloat4x4& Rec2020LinearToXYZ() noexcept;
    const MFloat4x4& XYZToRec2020Linear() noexcept;

    // Bradford chromatic adaptation: returns a 4x4 matrix (3x3 in upper-left, w=1 identity)
    // that adapts tristimulus values from srcWhite to dstWhite (given as CIE xy pairs).
    MFloat4x4 chromaticAdaptation(Float srcWhiteX, Float srcWhiteY, Float dstWhiteX, Float dstWhiteY) noexcept;
}
