#include "primitives/SplittingPlane.h"

using namespace vectorization;

namespace primitives
{

    const Float4 SIGN_MASK = blend<false, true, false, true>(NegZero<Float4>(), Zero<Float4>());

    SplittingPlane::SplittingPlane()
        :
        normalDistance(OneZ<Float4>())
    { }

    SplittingPlane::SplittingPlane(const Float4 & normalDistanceIn)
        :
        normalDistance(normalDistanceIn)
    { }

    SplittingPlane::SplittingPlane(
        const Float4 & origin,
        const Float4 & normal)
        :
        normalDistance(replaceW(normal, dot3(normal, origin)))
    { }

    const AxisAlignedBoundingBox bounding(const SplittingPlane & /* p */)
    {
        // infinite planes have infinite bounds
        return AxisAlignedBoundingBox();
    }

    // x = signed distance to plane
    // y = ray dot normal
    // z = -ray dot normal
    // w = ray dot normal
    inline const Float4 computePlaneIntersectionCoefficients(const Ray & r, const SplittingPlane & p)
    {
        const Float4 orientation = dot3v(r.direction, p.normalDistance);
        const Float4 signedDistance = dotv(r.origin, p.normalDistance) / orientation;
        return x_yzw(signedDistance, orientation) ^ SIGN_MASK;
    }

    // x = (signed distance to plane <= 0)
    // y = (ray dot normal <= 0)
    // z = (ray dot normal > 0)
    // w = (ray dot normal <= 0)
    inline const Float4::VectorBoolType testPlaneIntersectionCoefficients(const Float4 & c)
    {
        return c < Zero<Float4>();
    }

    const bool overlaps(const AxisAlignedBoundingBox & a, const SplittingPlane & p)
    {
        const Float4::VectorBoolType notNegative = p.normalDistance >= Zero<Float4>();
        const Float4 selmin = oneW(blendMasked(a.maximum, a.minimum, notNegative));
        const Float4 selmax = oneW(blendMasked(a.minimum, a.maximum, notNegative));
        return isNegative(dotv(p.normalDistance, selmin)) & !isNegative(dotv(p.normalDistance, selmax));
    }

    const bool overlaps(const Raycast & r, const SplittingPlane & p)
    {
        const Float4 c = computePlaneIntersectionCoefficients(r.ray, p);
        const Float4::VectorBoolType check = testPlaneIntersectionCoefficients(c);
        if (x(check | (zwzw(check) & backfaceCulledv(r)) | (yyyy(check) & frontfaceCulledv(r))))
        {
            return false;
        }
        return !outOfReach(r, x(c));
    }

    const Float nearestIntersection(const Raycast & r, const SplittingPlane & p, const Size2::ValueType originId)
    {
        const Float4 c = computePlaneIntersectionCoefficients(r.ray, p);
        const Float4::VectorBoolType check = testPlaneIntersectionCoefficients(c);
        if (x(check | (zwzw(check) & backfaceCulledv(r)) | (yyyy(check) & frontfaceCulledv(r))))
        {
            return r.maxDistance;
        }
        return select(
            selfOcclusion(r, originId, x(c)),
            r.maxDistance,
            x(c));
    }

}
