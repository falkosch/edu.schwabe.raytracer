#include "raytracing/shading/spectral/blackbody.h"

#include <vectorization.h>

namespace raytracer::spectral
{
    static constexpr double PLANCK_H = 6.62607015e-34;
    static constexpr double SPEED_C = 2.99792458e8;
    static constexpr double BOLTZMANN_K = 1.380649e-23;
    static constexpr double NANOMETERS_PER_METER = 1e9;

    // Planck's law with the wavelength in nanometers. In meters, lambda^5 (~1e-32) and the (1e-9)^5 conversion factor
    // (1e-45) sit at the edge of the float range, so a compiler that reassociates (clang's -fassociative-math) can
    // underflow or overflow them. The nanometer scale factors are folded into the constants, derived in double.
    // 2hc^2 * (1e9)^5, so that TWO_HC2_NM / lambda_nm^5 == 2hc^2 / lambda_m^5
    static constexpr Float TWO_HC2_NM = static_cast<Float>(
        2.0 * PLANCK_H * SPEED_C * SPEED_C * NANOMETERS_PER_METER * NANOMETERS_PER_METER * NANOMETERS_PER_METER *
        NANOMETERS_PER_METER * NANOMETERS_PER_METER
    );
    // hc/k * 1e9, so that HC_OVER_K_NM / (lambda_nm * T) == hc / (lambda_m * k * T)
    static constexpr Float HC_OVER_K_NM = static_cast<Float>(PLANCK_H * SPEED_C / BOLTZMANN_K * NANOMETERS_PER_METER);

    static constexpr Float EXP_CLAMP_MAX = 88.0f;
    static constexpr Float EXP_CLAMP_MIN = -88.0f;

    Float8 blackbody(const Float temperatureKelvin, const Float8& wavelengths) noexcept
    {
        if (temperatureKelvin <= Zero<Float>())
        {
            return Zero<Float8>();
        }

        const auto lambda5 = sqr(sqr(wavelengths)) * wavelengths;

        const auto numerator = Float8(TWO_HC2_NM) / lambda5;
        const auto exponent = Float8(HC_OVER_K_NM / temperatureKelvin) / wavelengths;
        const auto clampedExp = clamp(exponent, Float8(EXP_CLAMP_MIN), Float8(EXP_CLAMP_MAX));
        const auto denominator = exp(clampedExp) - One<Float8>();

        return numerator / denominator;
    }
}
