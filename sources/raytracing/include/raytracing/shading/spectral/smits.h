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

        // What rgbToIlluminant multiplies the basis spectra's sum by: D65, the white of sRGB, scaled so that the light
        // of white (1, 1, 1) has luminance 1
        Float8 illuminant;
    };

    SmitsBasis smitsBasisAt(const Float8& wavelengths) noexcept;

    Float8 rgbToSpectrum(const Float4& rgb, const SmitsBasis& basis) noexcept;

    Float8 rgbToSpectrum(const Float4& rgb, const Float8& wavelengths) noexcept;

    // Upsamples an RGB reflectance (a diffusion, a specular colour, the specular lobe's weight): rgbToSpectrum scaled
    // so that white (1, 1, 1) is 1 at its brightest wavelength (Smits' white is about 1.06 at every wavelength), and
    // clamped to [0, 1] at every wavelength: a surface reflects no more than it gets (pbrt-v3 scales its Smits
    // reflectances by 0.94 and clamps them). Lights take rgbToIlluminant, absorption lengths rgbToSpectrum.
    Float8 rgbToReflectance(const Float4& rgb, const SmitsBasis& basis) noexcept;

    // Upsamples an RGB light (an emittance, a background's radiance, the ambient light) as the light that an sRGB
    // surface of that colour reflects under D65: rgbToSpectrum times D65, so that white (1, 1, 1) is D65 of luminance 1
    // and displays as sRGB (1, 1, 1). Smits' spectra alone are illuminant E, whose white displays warm (sRGB-linear
    // 1.28, 1.01, 0.97 at luminance 1.06); pbrt-v3 upsamples lights by illuminant tables for the same reason. Values
    // above 1 scale the light, they aren't clamped.
    Float8 rgbToIlluminant(const Float4& rgb, const SmitsBasis& basis) noexcept;
}
