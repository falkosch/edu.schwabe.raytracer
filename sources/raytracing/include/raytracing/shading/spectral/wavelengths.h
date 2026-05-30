#pragma once

#include <vectorization.h>

namespace raytracer::spectral
{
    using namespace vectorization;

    inline constexpr Float LAMBDA_MIN = 380.0f;
    inline constexpr Float LAMBDA_MAX = 780.0f;
    inline constexpr Float LAMBDA_RANGE = 400.0f;
    inline constexpr ASizeT SPECTRAL_SAMPLES = 8;
    inline constexpr Float DELTA_LAMBDA = LAMBDA_RANGE / static_cast<Float>(SPECTRAL_SAMPLES);

    using SpectralVector = Float8;

    Float wrapToVisible(Float lambda) noexcept;

    Float8 wavelengthsAt(Float heroLambda) noexcept;
}
