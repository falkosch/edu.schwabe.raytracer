#pragma once

#include "EnvironmentProjection.h"

#include <vectorization.h>

namespace raytracer
{
    // Texture coordinates (u, v, 0, 0) in [0, 1] of the unit direction in an environment image of the projection.
    // horizonRadius is the radius of the horizon in the image for AllSkyFisheye; the other projections ignore it.
    Float4 environmentCoordinates(
        EnvironmentProjection projection, const Float4& direction, Float horizonRadius = Half<Float>()
    ) noexcept;
}
