#include "primitives/Facet.h"

using namespace vectorization;

namespace primitives
{
    Facet::Facet() noexcept
        : v0(), v1(), v2() { }

    Facet::Facet(const Float4 & v) noexcept
        : v0(v), v1(v), v2(v) { }

    Facet::Facet(const Float4 & v0In, const Float4 & v1In, const Float4 & v2In) noexcept
        : v0(v0In), v1(v1In), v2(v2In) { }

    const AxisAlignedBoundingBox bounding(const Facet & facet) noexcept {
        return extendBy(AxisAlignedBoundingBox(facet.v0, facet.v1), facet.v2);
    }

    const Facet operator+(const Facet & a, const Float4 & b) noexcept {
        return Facet(a.v0 + b, a.v1 + b, a.v2 + b);
    }

    const Facet operator-(const Facet & a, const Float4 & b) noexcept {
        return Facet(a.v0 - b, a.v1 - b, a.v2 - b);
    }

    const Float4 baryCenter(const Float4 & uv, const Facet & facet) noexcept {
        return facet.v0 + xxxx(uv) * (facet.v1 - facet.v0)
            + yyyy(uv) * (facet.v2 - facet.v0);
    }

    const Float4 nearestIntersectionHavel(
        const FacetNormals & planeNormals,
        const Raycast & raycast,
        const Float4 & maxDistance
    ) noexcept {
        auto determinant = dot3v(planeNormals.v0, raycast.ray.direction);
        auto determinantIsPositive = !isNegative(determinant);
        auto backCulled = (!determinantIsPositive) & backfaceCulled(raycast);
        auto frontCulled = determinantIsPositive & frontfaceCulled(raycast);

        if (backCulled | frontCulled) {
            auto det_t = -dotv(planeNormals.v0, raycast.ray.origin);

            if (!isNegative(det_t ^ (determinant * maxDistance - det_t))) {
                auto det_p = raycast.ray.origin * determinant + raycast.ray.direction * det_t;
                auto det_u = dotv(det_p, planeNormals.v1);

                if (!isNegative(det_u ^ (determinant - det_u))) {
                    auto det_v = dotv(det_p, planeNormals.v2);

                    if (!isNegative(det_v ^ (determinant - (det_u + det_v)))) {
                        return xz_xz(xy_xy(det_u, det_v), det_t) / determinant;
                    }
                }
            }
        }

        return maxDistance;
    }
}