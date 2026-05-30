#pragma once

#include <vectorization.h>

namespace raytracer::spectral
{
    using namespace vectorization;

    Float8 rgbToSpectrum(const Float4& rgb, const Float8& wavelengths) noexcept;
}
