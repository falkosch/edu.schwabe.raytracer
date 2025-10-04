#include "primitives/FacetEdges.h"

using namespace vectorization;

namespace primitives {
  FacetEdges::FacetEdges() noexcept = default;

  FacetEdges::FacetEdges(const Float4 &edge1, const Float4 &edge2) noexcept : edge1(edge1), edge2(edge2) {
  }

  Float4 nearestIntersectionMoeller(
      const Float4 &origin, const FacetEdges &facetEdges, const Ray &ray, const Float4 &maxDistance
  ) noexcept {
    // calculate direction from v0 to ray.origin
    const auto tv = ray.origin - origin;

    // calculate determinant (also part of the barycentric-U-coord)
    const auto pv = cross3(ray.direction, facetEdges.edge2);
    const auto determinant = reciprocal(dotv(facetEdges.edge1, pv));

    // triangle bound check on U
    const auto u = dotv(tv, pv) * determinant;
    const auto checkUBelow = u < Zero<Float4>();
    if (const auto checkUAbove = u > One<Float4>(); allTrue(checkUBelow | checkUAbove)) {
      return maxDistance;
    }

    // triangle bound check on V
    const auto qv = cross3(tv, facetEdges.edge1);
    const auto v = dotv(ray.direction, qv) * determinant;
    const auto uv = u + v;
    const auto checkVAbove = uv > One<Float4>();
    if (const auto checkVBelow = v < Zero<Float4>(); allTrue(checkVBelow | checkVAbove)) {
      return maxDistance;
    }

    // calculate d in "ray.origin + d * ray.direction"
    const auto d = dotv(facetEdges.edge2, qv) * determinant;
    return x_yzw(u, xy_zw(v, d));
  }
}
