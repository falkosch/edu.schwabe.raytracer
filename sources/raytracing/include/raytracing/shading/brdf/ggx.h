#pragma once

#include <vectorization.h>

#include "raytracing/shading/spectral/wavelengths.h"

namespace raytracer::brdf
{
    using namespace vectorization;

    // The roughness (the GGX alpha) up to which a reflection is a mirror: one ray in the mirrored direction instead of
    // glossy samples, and area lights show by their emissive object instead of a highlight
    constexpr Float MirrorRoughness = 0.02f;

    // Trowbridge-Reitz (GGX) normal distribution function
    Float ggxD(Float NoH, Float alpha) noexcept;

    // Smith height-correlated geometry term for GGX
    Float ggxG_smith(Float NoV, Float NoL, Float alpha) noexcept;

    // Smith height-correlated visibility term for GGX: G / (4 * NoV * NoL), with the NoV * NoL factor of G cancelled.
    // NoV and NoL must be positive.
    Float ggxV_smith(Float NoV, Float NoL, Float alpha) noexcept;

    // Full GGX microfacet BRDF: D * G * F / (4 * NoV * NoL)
    // N, V, L are unit vectors; alpha is roughness^2; F0 is reflectance at normal incidence (RGB).
    Float4 evaluateGGX(const Float4& N, const Float4& V, const Float4& L, const Float4& F0, Float alpha) noexcept;

    // GGX microfacet BRDF without Fresnel: D * G / (4 * NoV * NoL). Takes the cosines of the normal with the view
    // direction, the light direction and the half vector, clamped to >= 0, so callers can reuse cosines they already
    // have (NoV is the same for every light, NoL is the Lambert term).
    Float evaluateGGX_DG(Float NoV, Float NoL, Float NoH, Float alpha) noexcept;

    // Per-wavelength Schlick Fresnel: F0 and result are per-wavelength coefficients [0,1].
    // cosTheta is uniform across wavelengths (purely angular), passed as a SpectralVector so
    // callers can widen broadcast Float4 dot products directly without going through scalar.
    spectral::SpectralVector schlickFresnelSpectral(const spectral::SpectralVector& F0,
                                                    const spectral::SpectralVector& cosTheta) noexcept;

    // VNDF importance sampling (Heitz 2018): given surface normal N, view direction V,
    // roughness alpha, and two uniform random numbers xi (in xy), returns a sampled light direction.
    Float4 sampleGGX(const Float4& N, const Float4& V, const Float4& xi, Float alpha) noexcept;

    // The microfacet normal of sampleGGX: a normal of the GGX distribution of visible normals seen from V (Heitz 2018),
    // for rough refraction (Walter et al. 2007) as well as reflection
    Float4 sampleGGXNormal(const Float4& N, const Float4& V, const Float4& xi, Float alpha) noexcept;

    // Smith's masking function G1 of the GGX distribution for one direction, from its cosine to the normal (positive)
    Float smithG1(Float cosine, Float alpha) noexcept;

    // The density per solid angle of the directions sampleGGX returns: G1(V) * D(H) / (4 * NoV) for the half vector H
    // of V and the direction. NoV must be positive; directions below the surface have a density too.
    Float pdfGGX(Float NoV, Float NoH, Float alpha) noexcept;
}
