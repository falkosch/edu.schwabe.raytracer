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

    // Upsamples an RGB reflectance (a diffusion, a specular colour, the specular lobe's weight): rgbToSpectrum scaled
    // so that white (1, 1, 1) is 1 at its brightest wavelength (Smits' white is about 1.06 at every wavelength), and
    // clamped to [0, 1] at every wavelength: a surface reflects no more than it gets (pbrt-v3 scales its Smits
    // reflectances by 0.94 and clamps them). Lights and absorption lengths take rgbToSpectrum as they are.
    Float8 rgbToReflectance(const Float4& rgb, const SmitsBasis& basis) noexcept;
}
