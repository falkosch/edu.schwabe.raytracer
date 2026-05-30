#pragma once

#include "wavelengths.h"

namespace raytracer
{
    struct RGBS;
}

namespace raytracer::spectral
{
    using namespace vectorization;

    struct Spectrum
    {
        Float8 data;

        static Spectrum zero() noexcept;

        static Spectrum constant(Float c) noexcept;
    };

    Spectrum monochromatic(Float heroLambda, Float targetLambda, Float intensity) noexcept;

    Spectrum fromRGB(Float heroLambda, const Float4& rgb) noexcept;

    // Variant that takes pre-computed wavelengths (skips wavelengthsAt). Use when sampling
    // multiple RGB inputs at the same heroLambda — hoist wavelengthsAt out of the loop.
    Spectrum fromRGB(const Float8& wavelengths, const Float4& rgb) noexcept;

    Spectrum fromRGBS(Float heroLambda, const RGBS& rgb) noexcept;

    Float spectralMax(const SpectralVector& v) noexcept;

    Float spectralMax(const Spectrum& s) noexcept;

    SpectralVector spectralExp(const SpectralVector& v) noexcept;

    Spectrum spectralMix(const Spectrum& a, const Spectrum& b, const SpectralVector& factor) noexcept;

    Spectrum operator+(const Spectrum& a, const Spectrum& b) noexcept;

    Spectrum operator-(const Spectrum& a, const Spectrum& b) noexcept;

    Spectrum operator-(const Spectrum& a) noexcept;

    Spectrum operator*(const Spectrum& a, const Spectrum& b) noexcept;

    Spectrum operator*(const Spectrum& a, const SpectralVector& weights) noexcept;

    Spectrum operator*(const Spectrum& a, Float scalar) noexcept;

    Spectrum operator*(Float scalar, const Spectrum& a) noexcept;

    Spectrum& operator+=(Spectrum& a, const Spectrum& b) noexcept;

    Spectrum& operator*=(Spectrum& a, const Spectrum& b) noexcept;

    Spectrum& operator*=(Spectrum& a, Float scalar) noexcept;
}
