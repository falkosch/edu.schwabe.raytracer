#pragma once

#include "../geometry/SceneIntersection.h"

#include <primitives.h>

namespace raytracer {
  using namespace vectorization;
  using namespace primitives;

  /**
   * Adds visibility properties to a RayCast
   */
  struct Raytrace {
    RayCast rayCast;

    const SceneIntersection *originIntersection;

    ASizeT traceDepth;

    Float visibilityIndex;

    Raytrace();

    explicit Raytrace(
        const RayCast &rayCast, const SceneIntersection *originIntersection, ASizeT traceDepth, Float visibilityIndex
    );
  };
}
