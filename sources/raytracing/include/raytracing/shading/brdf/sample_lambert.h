#pragma once

#include <vectorization.h>

namespace raytracer::brdf
{
    using namespace vectorization;

    // Importance sampling of Lambert's BRDF: a direction around the unit normal N with the density cos(theta) / pi per
    // solid angle, from two uniform numbers xi (in xy). Malley's method: a point of the unit disk, by Shirley and
    // Chiu's concentric mapping (which keeps the strata of xi compact), lifted onto the hemisphere.
    Float4 sampleLambert(const Float4& N, const Float4& xi) noexcept;
}
