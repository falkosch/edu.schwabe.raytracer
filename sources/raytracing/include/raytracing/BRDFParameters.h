#pragma once

#include "geometry/SceneIntersection.h"

#include "shading/LightShading.h"
#include "shading/SurfaceShading.h"

namespace raytracer {
  using namespace vectorization;

  struct BRDFParameters {
    // Intersection information of ray with a scene-element
    SceneIntersection intersection;

    // Sampled surface properties at scene-intersection
    SurfaceShading surface;

    // Sampled lighting properties at scene-intersection
    LightShading lighting;

    // Direction of transmitted ray
    Float4 transmissionDirection;

    // Fresnel reflectance coefficient = 1 - transmittance coefficient
    Float4 reflectanceCoefficient;

    // Light absorption weight in material
    Float4 fractionTransmitted;

    // Distance from ray-origin to scene-intersection
    Float viewDistance;

    BRDFParameters();
  };
}
