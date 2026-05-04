#pragma once

#include <vectorization.h>

namespace raytracer::color
{
    using namespace vectorization;

    // ACES filmic tone mapping (Narkowicz 2015 fit of the RRT+ODT)
    // Input: linear RGB (any working space), Output: display-referred [0,1]
    Float4 acesFilmic(const Float4& linearRGB) noexcept;

    // Simplified AgX view transform (Sobotka): input matrix, log2 encode, polynomial sigmoid, output matrix.
    // Input: linear RGB, Output: display-referred [0,1]
    Float4 agx(const Float4& linearRGB) noexcept;

    // IEC 61966-2-1 sRGB transfer functions (piecewise: linear segment + gamma curve)
    Float4 srgbEncode(const Float4& linear) noexcept;
    Float4 srgbDecode(const Float4& encoded) noexcept;
}
