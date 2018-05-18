#include "primitives/BoundingSphere.h"
#include "../stdafx.h"

namespace primitives
{

    BoundingSphere::BoundingSphere()
        :
        centerRadius(OneW<Float4>())
    { }

    BoundingSphere::BoundingSphere(const Float4 & center, const Float radius)
        :
        centerRadius(replaceW(center, radius))
    { }

    const AxisAlignedBoundingBox bounding(const BoundingSphere & b)
    {
        const Float4 r = wwww(b.centerRadius);
        return AxisAlignedBoundingBox(oneW(b.centerRadius - r), oneW(b.centerRadius + r));
    }

    inline const Float4 computeSphereIntersectionCoefficients(const Ray & r, const BoundingSphere & b)
    {
        // geometric method from Real-Time Rendering 3: http://books.google.de/books?id=V1k1V9Ra1FoC&pg=PA741
        const Float4 sqrRadius = wwww(b.centerRadius * b.centerRadius);
        const Float4 vl = zeroW(b.centerRadius - r.origin);
        const Float4 s = dotv(vl, r.direction);
        const Float4 ll = dotv(vl, vl);
        const Float4 sqrM = ll - s * s;
        if ((isNegative(s) & x(ll > sqrRadius)) | x(sqrM > sqrRadius))
        {
            return NegInfinity<Float4>();
        }

        const Float4 q = sqrt(sqrRadius - sqrM);
        //d.x = d.z = s - q;
        //d.y = d.w = s + q;
        const Float4 d = subadd(s, q);
        return blend<false, true, false, true>(min(d, yyww(d)), max(xxzz(d), d));
    }

    inline const Float4::VectorBoolType testSphereIntersectionCoefficients(const Float4 & c)
    {
        // x = t0 < 0 = c.x < 0
        // y = t1 < 0 = c.y < 0
        // z = t0 < 0 = c.z < 0
        // w = t1 < 0 = c.w < 0
        return c < Zero<Float4>();
    }

    const bool overlaps(const Raycast & r, const BoundingSphere & b)
    {
        const Float4 c = computeSphereIntersectionCoefficients(r.ray, b);
        const Float4::VectorBoolType check = testSphereIntersectionCoefficients(c);
        // overlaps if c.y >= 0 and (c.x < 0 or ray reaches front of sphere)
        return !!x(andnot(andnot(check, outOfReach(r, c)), !yyyy(check)));
    }

    const Float nearestIntersection(const Raycast & r, const BoundingSphere & b, const Size2::ValueType originId)
    {
        const Float4 c = computeSphereIntersectionCoefficients(r.ray, b);
        const Float4::VectorBoolType check = testSphereIntersectionCoefficients(c);

        // raycast starts behind sphere
        if (y(check))
        {
            return r.maxDistance;
        }

        // reject t.y if backface culled
        const Float4 ty = yyww(c);
        const Float4::VectorBoolType selfOccludedY = convert<Float4::VectorBoolType>(selfOcclusion(r, originId, x(ty)));
        const Float4 tt = blendMasked(ty, Float4(r.maxDistance), selfOccludedY | backfaceCulledv(r));

        // reject t.x if frontfaceCulled or overlaps or self occluded
        const Float4::VectorBoolType selfOccludedX = convert<Float4::VectorBoolType>(selfOcclusion(r, originId, x(c)));
        return x(blendMasked(c, tt, selfOccludedX | frontfaceCulledv(r)));
    }

}
