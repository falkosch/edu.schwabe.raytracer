#pragma once

#include "Form.h"

namespace raytracer {
  using namespace vectorization;
  using namespace primitives;

  struct Sphere : public Form {
    BoundingSphere bounding;

    Sphere();

    explicit Sphere(const BoundingSphere &bounding);

    explicit Sphere(const Float4 &center, const Float radius);

    virtual ~Sphere();

    const AxisAlignedBoundingBox getBounding() const;

    const Float findNearestIntersection(
        const RayCast &rayCast, const FacetIntersection *const originIntersection, FacetIntersection &intersectionOut
    ) const;

    const Float findAnyIntersection(
        const RayCast &rayCast, const FacetIntersection *const originIntersection, FacetIntersection &intersectionOut
    ) const;

    const Float getIndividualIntersectionCosts() const {
      // benchmarked intersection costs in cycles
      return 37.8098f;
    }
  };
}
