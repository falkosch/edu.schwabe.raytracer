#pragma once

#include "AxisAlignedBoundingBox.h"
#include "RayCast.h"

namespace primitives {
  using namespace vectorization;

  struct Facet {
    Float4 v0, v1, v2;

    Facet() noexcept;

    explicit Facet(const Float4 &v) noexcept;

    explicit Facet(const Float4 &v0, const Float4 &v1, const Float4 &v2) noexcept;
  };

  typedef Facet FacetTexCoords4, FacetNormals;

  const AxisAlignedBoundingBox bounding(const Facet &facet) noexcept;

  const Facet operator+(const Facet &a, const Float4 &b) noexcept;

  const Facet operator-(const Facet &a, const Float4 &b) noexcept;

  // Computes the bary centric point given coordinates { u, v } and a Facet.
  const Float4 baryCenter(const Float4 &uv, const Facet &facet) noexcept;

  /*
   * Returns { u, v, distance } if ray intersects with facet given by a
   * facet's origin vertex and its edges. Outputs u and v are the barycenter
   * texture coords at the intersection. If there is no intersection, the
   * return value is equal to maxDistance.
   */
  const Float4 nearestIntersectionHavel(
      const FacetNormals &planeNormals, const RayCast &rayCast, const Float4 &maxDistance
  ) noexcept;
}
