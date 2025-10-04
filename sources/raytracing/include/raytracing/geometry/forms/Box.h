#pragma once

#include "Form.h"

namespace raytracer {
  using namespace vectorization;
  using namespace primitives;

  struct Box : Form {
    AxisAlignedBoundingBox bounding;

    Box();
    explicit Box(const AxisAlignedBoundingBox &bounding);
    explicit Box(const Float4 &min, const Float4 &max);

    ~Box() override;

    AxisAlignedBoundingBox getBounding() const override;

    Float findNearestIntersection(
        const RayCast &rayCast, const FacetIntersection *originIntersection, FacetIntersection &intersectionOut
    ) const override;

    Float findAnyIntersection(
        const RayCast &rayCast, const FacetIntersection *originIntersection, FacetIntersection &intersectionOut
    ) const override;

    Float getIndividualIntersectionCosts() const override {
      return 19.8722f;
    }
  };
}
