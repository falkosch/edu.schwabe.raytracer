#pragma once

#include <vectorization.h>

namespace raytracer::spectral
{
    using namespace vectorization;

    // Smits 1999 basis spectra sampled at one set of wavelengths. Every RGB-to-spectrum upsampling at these wavelengths
    // is a weighted sum of three of them, so computing them once makes each upsampling a few multiply-adds.
    struct SmitsBasis
    {
        Float8 white;
        Float8 cyan;
        Float8 magenta;
        Float8 yellow;
        Float8 red;
        Float8 green;
        Float8 blue;
    };

    SmitsBasis smitsBasisAt(const Float8& wavelengths) noexcept;

    Float8 rgbToSpectrum(const Float4& rgb, const SmitsBasis& basis) noexcept;

    Float8 rgbToSpectrum(const Float4& rgb, const Float8& wavelengths) noexcept;
}
