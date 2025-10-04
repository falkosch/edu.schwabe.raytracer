#include "primitives/Facet.h"

using namespace vectorization;

namespace primitives {
  Facet::Facet() noexcept = default;

  Facet::Facet(const Float4 &v) noexcept : v0(v), v1(v), v2(v) {
  }

  Facet::Facet(const Float4 &v0, const Float4 &v1, const Float4 &v2) noexcept : v0(v0), v1(v1), v2(v2) {
  }

  AxisAlignedBoundingBox bounding(const Facet &facet) noexcept {
    return extendBy(AxisAlignedBoundingBox(facet.v0, facet.v1), facet.v2);
  }

  Facet operator+(const Facet &a, const Float4 &b) noexcept {
    return Facet(a.v0 + b, a.v1 + b, a.v2 + b);
  }

  Facet operator-(const Facet &a, const Float4 &b) noexcept {
    return Facet(a.v0 - b, a.v1 - b, a.v2 - b);
  }

  Float4 baryCenter(const Float4 &uv, const Facet &facet) noexcept {
    return facet.v0 + xxxx(uv) * (facet.v1 - facet.v0) + yyyy(uv) * (facet.v2 - facet.v0);
  }

  // Havel et al. ray-triangle intersection test as in paper "Yet Faster Ray-Triangle Intersection (Using SSE4)", 2009.
  Float4 nearestIntersectionHavel(
      const FacetNormals &planeNormals, const RayCast &rayCast, const Float4 &maxDistance
  ) noexcept {
    const auto determinant = dot3v(planeNormals.v0, rayCast.ray.direction);
    const auto detT = -dotv(planeNormals.v0, rayCast.ray.origin);

    // no hit when determinants have different signs
    if (const auto checkT = determinant * maxDistance - detT; isNegative(detT ^ checkT)) {
      return maxDistance;
    }

    const auto intersectionPoint = rayCast.ray.origin * determinant + rayCast.ray.direction * detT;
    const auto detU = dotv(intersectionPoint, planeNormals.v1);

    if (const auto checkU = determinant - detU; isNegative(detU ^ checkU)) {
      return maxDistance;
    }

    const auto detV = dotv(intersectionPoint, planeNormals.v2);
    if (const auto checkV = determinant - (detU + detV); isNegative(detV ^ checkV)) {
      return maxDistance;
    }

    return xz_xz(xy_xy(detU, detV), detT) / determinant;
  }
}
