#pragma once

#include "Form.h"

namespace raytracer {
  using namespace vectorization;
  using namespace primitives;

  class Plane : public Form {
    void reconstructFacetEdgesFromPlaneEquation(const Float4 &planeEquation);

  public:
    SplittingPlane plane;
    FacetEdges baseVectors;

    Plane();
    explicit Plane(const Float4 &planeEquation);
    explicit Plane(const Float4 &origin, const Float4 &normal);
    explicit Plane(const Float4 &origin, const FacetEdges &baseVectors);

    ~Plane() override;

    bool isInfinite() const override;

    Float findNearestIntersection(
        const RayCast &rayCast, const FacetIntersection *originIntersection, FacetIntersection &intersectionOut
    ) const override;

    Float findAnyIntersection(
        const RayCast &rayCast, const FacetIntersection *originIntersection, FacetIntersection &intersectionOut
    ) const override;

    Float getIndividualIntersectionCosts() const override {
      return 18.5633f;
    }
  };
}
