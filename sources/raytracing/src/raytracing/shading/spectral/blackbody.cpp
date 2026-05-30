#include "raytracing/shading/spectral/blackbody.h"

#include <vectorization.h>

namespace raytracer::spectral
{
    static constexpr Float PLANCK_H = 6.62607015e-34f;
    static constexpr Float SPEED_C = 2.99792458e8f;
    static constexpr Float BOLTZMANN_K = 1.380649e-23f;
    static constexpr Float NANOMETERS_TO_METERS = 1e-9f;

    static constexpr Float TWO_HC2 = 2.0f * PLANCK_H * SPEED_C * SPEED_C;
    static constexpr Float HC_OVER_K = PLANCK_H * SPEED_C / BOLTZMANN_K;

    static constexpr Float EXP_CLAMP_MAX = 88.0f;
    static constexpr Float EXP_CLAMP_MIN = -88.0f;

    Float8 blackbody(const Float temperatureKelvin, const Float8& wavelengths) noexcept
    {
        if (temperatureKelvin <= Zero<Float>())
        {
            return Zero<Float8>();
        }

        const auto lambda_m = wavelengths * NANOMETERS_TO_METERS;

        const auto lambda5 = sqr(sqr(lambda_m)) * lambda_m;

        const auto numerator = Float8(TWO_HC2) / lambda5;
        const auto exponent = Float8(HC_OVER_K / temperatureKelvin) / lambda_m;
        const auto clampedExp = clamp(exponent, Float8(EXP_CLAMP_MIN), Float8(EXP_CLAMP_MAX));
        const auto denominator = exp(clampedExp) - One<Float8>();

        return numerator / denominator;
    }
}
