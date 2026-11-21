#pragma once

#include "../common/RGBS.h"

#include "spectral/conversion.h"
#include "spectral/smits.h"
#include "spectral/spectrum.h"

#include <memory>
#include <vector>

namespace raytracer
{
    using namespace vectorization;

    struct LightInfo;

    // Spectral data that depends only on a hero wavelength and the scene's lights. Every ray of a path keeps the hero
    // wavelength of its primary ray, and that depends only on the pixel sub-sample, so the raytracer builds one context
    // per sub-sample and frame. Rays refer to it instead of re-deriving wavelengths, RGB upsampling bases and light
    // spectra at every hit.
    struct SpectralContext
    {
        using SpectrumList = std::vector<spectral::Spectrum, AlignedAllocator<spectral::Spectrum>>;

        Float heroLambda;

        // The 8 hero-sampled wavelengths
        Float8 wavelengths;

        // Smits basis spectra at the wavelengths, for RGB-to-spectrum upsampling
        spectral::SmitsBasis smitsBasis;

        // Colour-matching functions at the wavelengths, for projecting path radiance to XYZ
        spectral::XYZContext xyz;

        // Upsampled ambient light; zero for a context without scene lighting
        spectral::Spectrum ambient;

        // Emitted spectrum of each light, in SceneShader::getLights() order; empty for a context without scene lighting
        SpectrumList lightEmittance;

        // Wavelength-dependent data only, without scene lighting.
        explicit SpectralContext(Float heroLambda);

        SpectralContext(
            Float heroLambda, const RGBS& ambientLight, const std::vector<std::unique_ptr<LightInfo>>& lights
        );

        // Upsamples an RGB value to a Spectrum at the context's wavelengths.
        spectral::Spectrum fromRGB(const Float4& rgb) const noexcept;
    };
}
