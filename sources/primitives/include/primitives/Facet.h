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

  AxisAlignedBoundingBox bounding(const Facet &facet) noexcept;

  Facet operator+(const Facet &a, const Float4 &b) noexcept;
  Facet operator-(const Facet &a, const Float4 &b) noexcept;

  /**
   * @param uv coordinates of the barycenter
   * @param facet facet with normals
   * @return bary centric point
   */
  Float4 baryCenter(const Float4 &uv, const Facet &facet) noexcept;

  /**
   * @param planeNormals facet's origin vertex and its edges
   * @param rayCast
   * @param maxDistance
   * @return { u, v, distance } if the ray intersects with facet, u and v are coordinates of the barycenter at the
   * intersection. If there is no intersection, the return value is equal to maxDistance.
   */
  Float4 nearestIntersectionHavel(
      const FacetNormals &planeNormals, const RayCast &rayCast, const Float4 &maxDistance
  ) noexcept;
}
