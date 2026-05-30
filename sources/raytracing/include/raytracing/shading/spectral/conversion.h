#pragma once

#include "raytracing/shading/spectral/cmf.h"
#include "raytracing/shading/spectral/spectrum.h"
#include "raytracing/common/RGBS.h"

namespace raytracer::spectral
{
    // XYZ integration context: depends only on heroLambda, so it can be reused across
    // multiple spectrumToXYZ calls that share heroLambda.
    struct XYZContext
    {
        CmfSample cmf;
        Float rcpYBarSum;
    };

    XYZContext makeXYZContext(Float heroLambda) noexcept;

    Float4 spectrumToXYZ(const Spectrum& s, const XYZContext& ctx) noexcept;

    RGBS xyzToDisplayRGB(const Float4& xyz) noexcept;
}
