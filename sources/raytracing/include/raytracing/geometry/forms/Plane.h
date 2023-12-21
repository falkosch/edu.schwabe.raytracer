#pragma once

#include "Form.h"

namespace raytracer {
  using namespace vectorization;
  using namespace primitives;

  class Plane : public Form {
    void reconstructFacetEdgesFromPlaneEquation(const Float4 &planeEqu);

  public:
    SplittingPlane plane;

    FacetEdges baseVectors;

    Plane();

    explicit Plane(const Float4 &planeEquation);

    explicit Plane(const Float4 &origin, const Float4 &normal);

    explicit Plane(const Float4 &origin, const FacetEdges &baseVectors);

    virtual ~Plane();

    const bool isInfinite() const;

    const Float findNearestIntersection(
        const RayCast &rayCast, const FacetIntersection *const originIntersection, FacetIntersection &intersectionOut
    ) const;

    const Float findAnyIntersection(
        const RayCast &rayCast, const FacetIntersection *const originIntersection, FacetIntersection &intersectionOut
    ) const;

    const Float getIndividualIntersectionCosts() const {
      // benchmarked intersection costs in cycles
      return 18.5633f;
    }
  };
}
