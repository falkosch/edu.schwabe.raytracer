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
        return Spectrum{rgbToSpectrum(rgb, wavelengths)};
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
