#pragma once

#include "Form.h"

namespace raytracer {
  using namespace vectorization;
  using namespace primitives;

  struct Sphere : Form {
    BoundingSphere bounding;

    Sphere();
    explicit Sphere(const BoundingSphere &bounding);
    explicit Sphere(const Float4 &center, Float radius);

    ~Sphere() override;

    AxisAlignedBoundingBox getBounding() const override;

    Float findNearestIntersection(
        const RayCast &rayCast, const FacetIntersection *originIntersection, FacetIntersection &intersectionOut
    ) const override;

    Float findAnyIntersection(
        const RayCast &rayCast, const FacetIntersection *originIntersection, FacetIntersection &intersectionOut
    ) const override;

    Float getIndividualIntersectionCosts() const override {
      return 37.8098f;
    }
  };
}
