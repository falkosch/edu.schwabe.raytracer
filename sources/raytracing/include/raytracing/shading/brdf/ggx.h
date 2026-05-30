#pragma once

#include <vectorization.h>

#include "raytracing/shading/spectral/wavelengths.h"

namespace raytracer::brdf
{
    using namespace vectorization;

    // Trowbridge-Reitz (GGX) normal distribution function
    Float ggxD(Float NoH, Float alpha) noexcept;

    // Smith height-correlated geometry term for GGX
    Float ggxG_smith(Float NoV, Float NoL, Float alpha) noexcept;

    // Full GGX microfacet BRDF: D * G * F / (4 * NoV * NoL)
    // N, V, L are unit vectors; alpha is roughness^2; F0 is reflectance at normal incidence (RGB).
    Float4 evaluateGGX(const Float4& N, const Float4& V, const Float4& L, const Float4& F0, Float alpha) noexcept;

    Float evaluateGGX_DG(const Float4& N, const Float4& V, const Float4& L, const Float4& H, Float alpha) noexcept;

    // Per-wavelength Schlick Fresnel: F0 and result are per-wavelength coefficients [0,1].
    // cosTheta is uniform across wavelengths (purely angular), passed as a SpectralVector so
    // callers can widen broadcast Float4 dot products directly without going through scalar.
    spectral::SpectralVector schlickFresnelSpectral(const spectral::SpectralVector& F0,
                                                    const spectral::SpectralVector& cosTheta) noexcept;

    // VNDF importance sampling (Heitz 2018): given surface normal N, view direction V,
    // roughness alpha, and two uniform random numbers xi (in xy), returns a sampled light direction.
    Float4 sampleGGX(const Float4& N, const Float4& V, const Float4& xi, Float alpha) noexcept;
}
