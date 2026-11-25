#include "raytracing/shading/spectral/spectrum.h"

#include "raytracing/shading/spectral/smits.h"

#include "raytracing/shading/spectral/wavelengths.h"
#include "raytracing/common/RGBS.h"

#include <vectorization.h>

namespace raytracer::spectral
{
    Spectrum Spectrum::zero() noexcept
    {
        return Spectrum{Zero<Float8>()};
    }

    Spectrum Spectrum::constant(const Float c) noexcept
    {
        return Spectrum{Float8(c)};
    }

    Spectrum monochromatic(const Float heroLambda, const Float targetLambda, const Float intensity) noexcept
    {
        const auto wavelengths = wavelengthsAt(heroLambda);
        const auto halfBin = Float8(DELTA_LAMBDA * Half<Float>());
        const auto dist = abs(wavelengths - Float8(targetLambda));
        const auto mask = dist < halfBin;
        return Spectrum{select(mask, Float8(intensity), Zero<Float8>())};
    }

    Spectrum fromRGB(const Float heroLambda, const Float4& rgb) noexcept
    {
        return fromRGB(wavelengthsAt(heroLambda), rgb);
    }

    Spectrum fromRGB(const Float8& wavelengths, const Float4& rgb) noexcept
    {
        return Spectrum{rgbToIlluminant(rgb, smitsBasisAt(wavelengths))};
    }

    Spectrum fromRGBS(const Float heroLambda, const RGBS& rgb) noexcept
    {
        return fromRGB(heroLambda, rgb.value);
    }

    Float spectralMax(const SpectralVector& v) noexcept
    {
        return x1(maxv(v));
    }

    Float spectralMax(const Spectrum& s) noexcept
    {
        return spectralMax(s.data);
    }

    Spectrum limitPeak(const Spectrum& s, const Float limit) noexcept
    {
        const auto peak = spectralMax(s);
        return limit > Zero<Float>() && peak > limit ? s * (limit / peak) : s;
    }

    Spectrum shiftWavelengths(const Spectrum& s, const Float steps) noexcept
    {
        alignas(YMM_ALIGNMENT) Float lanes[SPECTRAL_SAMPLES];
        store(s.data, lanes);
        // the lanes a step further in the shift's direction
        const auto ascending = steps >= Zero<Float>();
        const auto neighbours = ascending
                                    ? Float8(lanes[1], lanes[2], lanes[3], lanes[4], lanes[5], lanes[6], lanes[7],
                                             lanes[7])
                                    : Float8(lanes[0], lanes[0], lanes[1], lanes[2], lanes[3], lanes[4], lanes[5],
                                             lanes[6]);
        return Spectrum{multiplyAdd(neighbours - s.data, Float8(ascending ? steps : -steps), s.data)};
    }

    SpectralVector spectralExp(const SpectralVector& v) noexcept
    {
        return exp(v);
    }

    Spectrum spectralMix(const Spectrum& a, const Spectrum& b, const SpectralVector& factor) noexcept
    {
        return Spectrum{mix(a.data, b.data, factor)};
    }

    Spectrum operator+(const Spectrum& a, const Spectrum& b) noexcept
    {
        return Spectrum{a.data + b.data};
    }

    Spectrum operator-(const Spectrum& a, const Spectrum& b) noexcept
    {
        return Spectrum{a.data - b.data};
    }

    Spectrum operator-(const Spectrum& a) noexcept
    {
        return Spectrum{-a.data};
    }

    Spectrum operator*(const Spectrum& a, const Spectrum& b) noexcept
    {
        return Spectrum{a.data * b.data};
    }

    Spectrum operator*(const Spectrum& a, const SpectralVector& weights) noexcept
    {
        return Spectrum{a.data * weights};
    }

    Spectrum operator*(const Spectrum& a, const Float scalar) noexcept
    {
        return Spectrum{a.data * scalar};
    }

    Spectrum operator*(const Float scalar, const Spectrum& a) noexcept
    {
        return a * scalar;
    }

    Spectrum& operator+=(Spectrum& a, const Spectrum& b) noexcept
    {
        a.data = a.data + b.data;
        return a;
    }

    Spectrum& operator*=(Spectrum& a, const Spectrum& b) noexcept
    {
        a.data = a.data * b.data;
        return a;
    }

    Spectrum& operator*=(Spectrum& a, const Float scalar) noexcept
    {
        a.data = a.data * scalar;
        return a;
    }
}
