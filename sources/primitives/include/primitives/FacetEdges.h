#pragma once

#include "Ray.h"

#include <vectorization.h>

namespace primitives {
  using namespace vectorization;

  struct FacetEdges {
    // vector from A to B or "v1 - v0"
    Float4 edge1;
    // vector from A to C or "v2 - v0"
    Float4 edge2;

    FacetEdges() noexcept;

    explicit FacetEdges(const Float4 &edge1, const Float4 &edge2) noexcept;
  };

  /**
   * Returns { u, v, distance } if ray intersects with facet given by a
   * facet's origin vertex and its edges. Outputs u and v are the barycenter
   * texture coords at the intersection. If there is no intersection, the
   * return value is equal to maxDistance.
   */
  Float4 nearestIntersectionMoeller(
      const Float4 &v0, const FacetEdges &facetEdges, const Ray &ray, const Float4 &maxDistance
  ) noexcept;
}
