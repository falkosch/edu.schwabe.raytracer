#pragma once

#include <vectorization.h>

namespace raytracer {
  using namespace vectorization;

  template <typename RayType, typename IntersectionType>
  struct Intersectable {
    virtual ~Intersectable() {
    }

    virtual Float findNearestIntersection(
        const RayType &ray, const IntersectionType *originIntersection, IntersectionType &intersectionOut
    ) const = 0;

    virtual Float findAnyIntersection(
        const RayType &ray, const IntersectionType *originIntersection, IntersectionType &intersectionOut
    ) const = 0;
  };
}
