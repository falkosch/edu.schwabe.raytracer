#include "primitives/FacetEdges.h"

using namespace vectorization;

namespace primitives
{
    FacetEdges::FacetEdges() noexcept
        : edge1(), edge2() { }

    FacetEdges::FacetEdges(const Float4 & edge1, const Float4 & edge2) noexcept
        : edge1(edge1), edge2(edge2) { }

    const Float4 nearestIntersectionMoeller(
        const Float4 & v0,
        const FacetEdges & facetEdges,
        const Ray & ray,
        const Float4 & maxDistance
    ) noexcept {
        // calculate direction from v0 to ray.origin
        auto tvec = ray.origin - v0;

        // calculate determinant (also part of the barycentric-U-coord)
        auto pvec = cross3(ray.direction, facetEdges.edge2);
        auto det = reciprocal(dotv(facetEdges.edge1, pvec));

        // triangle bound check on U
        auto u = dotv(tvec, pvec) * det;
        if (allTrue((u < Zero<Float4>()) | (u > One<Float4>()))) {
            return maxDistance;
        }

        // triangle bound check on V
        auto qvec = cross3(tvec, facetEdges.edge1);
        auto v = dotv(ray.direction, qvec) * det;
        if (allTrue((v < Zero<Float4>()) | ((u + v) > One<Float4>()))) {
            return maxDistance;
        }

        // calculate t (in "orig + t * dir" == "ray.origin + t * ray.direction")
        return x_yzw(
            u,
            xy_zw(
                v,
                dotv(facetEdges.edge2, qvec) * det // == t
            )
        );
    }
}