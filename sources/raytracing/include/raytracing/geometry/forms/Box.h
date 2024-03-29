#pragma once

#include "Form.h"

namespace raytracer {
  using namespace vectorization;
  using namespace primitives;

  struct Box : public Form {
    AxisAlignedBoundingBox bounding;

    Box();

    explicit Box(const AxisAlignedBoundingBox &bounding);

    explicit Box(const Float4 &min, const Float4 &max);

    virtual ~Box();

    const AxisAlignedBoundingBox getBounding() const;

    const Float findNearestIntersection(
        const RayCast &rayCast, const FacetIntersection *const originIntersection, FacetIntersection &intersectionOut
    ) const;

    const Float findAnyIntersection(
        const RayCast &rayCast, const FacetIntersection *const originIntersection, FacetIntersection &intersectionOut
    ) const;

    const Float getIndividualIntersectionCosts() const {
      // benchmarked intersection costs in cycles
      return 19.8722f;
    }
  };
}
