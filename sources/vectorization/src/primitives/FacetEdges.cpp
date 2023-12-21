#include "primitives/FacetEdges.h"

using namespace vectorization;

namespace primitives {
  FacetEdges::FacetEdges() noexcept : edge1(), edge2() {
  }

  FacetEdges::FacetEdges(const Float4 &edge1, const Float4 &edge2) noexcept : edge1(edge1), edge2(edge2) {
  }

  const Float4 nearestIntersectionMoeller(
      const Float4 &v0, const FacetEdges &facetEdges, const Ray &ray, const Float4 &maxDistance
  ) noexcept {
    // calculate direction from v0 to ray.origin
    auto tv = ray.origin - v0;

    // calculate determinant (also part of the barycentric-U-coord)
    auto pv = cross3(ray.direction, facetEdges.edge2);
    auto determinant = reciprocal(dotv(facetEdges.edge1, pv));

    // triangle bound check on U
    auto u = dotv(tv, pv) * determinant;
    if (allTrue((u < Zero<Float4>()) | (u > One<Float4>()))) {
      return maxDistance;
    }

    // triangle bound check on V
    auto qv = cross3(tv, facetEdges.edge1);
    auto v = dotv(ray.direction, qv) * determinant;
    if (allTrue((v < Zero<Float4>()) | ((u + v) > One<Float4>()))) {
      return maxDistance;
    }

    // calculate tv in "ray.origin + tv * ray.direction"
    return x_yzw(
        u, xy_zw(
               v, dotv(facetEdges.edge2, qv) * determinant // == tv
           )
    );
  }
}