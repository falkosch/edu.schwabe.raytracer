#pragma once

#include <vectorization.h>

namespace raytracer::brdf
{
    using namespace vectorization;

    // Trowbridge-Reitz (GGX) normal distribution function
    Float ggxD(Float NoH, Float alpha) noexcept;

    // Smith height-correlated geometry term for GGX
    Float ggxG(Float NoV, Float NoL, Float alpha) noexcept;

    // Full GGX microfacet BRDF: D * G * F / (4 * NoV * NoL)
    // N, V, L are unit vectors; alpha is roughness^2; F0 is reflectance at normal incidence (RGB).
    Float4 evaluateGGX(const Float4& N, const Float4& V, const Float4& L, const Float4& F0, Float alpha) noexcept;

    // VNDF importance sampling (Heitz 2018): given surface normal N, view direction V,
    // roughness alpha, and two uniform random numbers xi (in xy), returns a sampled light direction.
    Float4 sampleGGX(const Float4& N, const Float4& V, const Float4& xi, Float alpha) noexcept;
}
