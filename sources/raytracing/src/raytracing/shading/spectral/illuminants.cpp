#include "raytracing/shading/spectral/illuminants.h"

#include "raytracing/shading/spectral/wavelengths.h"

#include <vectorization.h>

#include <cmath>

namespace raytracer::spectral
{
    static constexpr ASizeT TABLE_SIZE = 81;
    static constexpr Float TABLE_STEP = 5.0f;

    static constexpr Float g_D65[TABLE_SIZE] = {
        49.9755f, 52.3118f, 54.6482f, 68.7015f, 82.7549f,
        87.1204f, 91.4860f, 92.4589f, 93.4318f, 90.0570f,
        86.6823f, 95.7736f, 104.8650f, 110.9360f, 117.0080f,
        117.4100f, 117.8120f, 116.3360f, 114.8610f, 115.3920f,
        115.9230f, 112.3670f, 108.8110f, 109.0820f, 109.3540f,
        108.5780f, 107.8020f, 106.2960f, 104.7900f, 106.2390f,
        107.6890f, 106.0470f, 104.4050f, 104.2250f, 104.0460f,
        102.0230f, 100.0000f, 98.1671f, 96.3342f, 96.0611f,
        95.7880f, 92.2368f, 88.6856f, 89.3459f, 90.0062f,
        89.8026f, 89.5991f, 88.6489f, 87.6987f, 85.4936f,
        83.2886f, 83.4939f, 83.6992f, 81.8630f, 80.0268f,
        80.1207f, 80.2146f, 81.2462f, 82.2778f, 80.2810f,
        78.2842f, 74.0027f, 69.7213f, 70.6652f, 71.6091f,
        72.9790f, 74.3490f, 67.9765f, 61.6040f, 65.7448f,
        69.8856f, 72.4863f, 75.0870f, 69.3398f, 63.5927f,
        55.0054f, 46.4182f, 56.6118f, 66.8054f, 65.0941f,
        63.3828f
    };

    static constexpr Float ILLUMINANT_A_TEMP = 2856.0f;
    static constexpr Float PLANCK_C2 = 1.4388e7f;
    static constexpr Float NORMALIZATION_WAVELENGTH = 560.0f;
    static constexpr Float NORMALIZATION_VALUE = 100.0f;

    static Float illuminantAAt(const Float wavelength) noexcept
    {
        const Float ratio = NORMALIZATION_WAVELENGTH / wavelength;
        const Float expRef = std::exp(PLANCK_C2 / (ILLUMINANT_A_TEMP * NORMALIZATION_WAVELENGTH));
        const Float expLam = std::exp(PLANCK_C2 / (ILLUMINANT_A_TEMP * wavelength));
        const Float ratio5 = ratio * ratio * ratio * ratio * ratio;
        return NORMALIZATION_VALUE * ratio5 * (expRef - One<Float>()) / (expLam - One<Float>());
    }

    static Float lookupD65(const Float wavelength) noexcept
    {
        const Float idx = max(Zero<Float>(), (wavelength - LAMBDA_MIN) / TABLE_STEP);
        const ASizeT i0 = min(static_cast<ASizeT>(idx), TABLE_SIZE - 2);
        const Float frac = idx - static_cast<Float>(i0);
        return mix(g_D65[i0], g_D65[i0 + 1], frac);
    }

    Float8 illuminantAt(const Illuminant illuminant, const Float8& wavelengths) noexcept
    {
        const Float wl[SPECTRAL_SAMPLES] = {
            x1(wavelengths), x2(wavelengths), x3(wavelengths), x4(wavelengths),
            x5(wavelengths), x6(wavelengths), x7(wavelengths), x8(wavelengths)
        };

        Float result[SPECTRAL_SAMPLES]{};
        for (ASizeT i = 0; i < SPECTRAL_SAMPLES; ++i)
        {
            switch (illuminant)
            {
            case Illuminant::D65:
                result[i] = lookupD65(wl[i]);
                break;
            case Illuminant::A:
                result[i] = illuminantAAt(wl[i]);
                break;
            }
        }

        return Float8(result);
    }
}
