#include "../stdafx.h"

#include "primitives/AxisAlignedBoundingBox.h"

using namespace vectorization;

namespace primitives
{

    AxisAlignedBoundingBox::AxisAlignedBoundingBox()
        :
        minimum(Float_Limits::max()),
        maximum(Float_Limits::lowest())
    { }

    AxisAlignedBoundingBox::AxisAlignedBoundingBox(
        const Float4 & a,
        const Float4 & b)
        :
        minimum(min(a, b)),
        maximum(max(a, b))
    { }

    const Float4 extents(const AxisAlignedBoundingBox & a)
    {
        return a.maximum - a.minimum;
    }

    const Float4 halfLengths(const AxisAlignedBoundingBox & a)
    {
        return extents(a) * Half<Float4>();
    }

    const Float4 center(const AxisAlignedBoundingBox & a)
    {
        return center(a, halfLengths(a));
    }

    const Float4 center(const AxisAlignedBoundingBox & a, const Float4 & halfLengths)
    {
        return halfLengths + a.minimum;
    }

    const Float surfaceArea(const AxisAlignedBoundingBox & a)
    {
        return surfaceArea(a, extents(a));
    }

    const Float surfaceArea(const AxisAlignedBoundingBox & /* a */, const Float4 & extents)
    {
        const Float4 t = zzyy(extents) * yxxy(extents);
        return horizontalSum3(t + t);
    }

    const AxisAlignedBoundingBox extendBy(const AxisAlignedBoundingBox & a, const Float4 & p)
    {
        AxisAlignedBoundingBox r;
        r.minimum = min(p, a.minimum);
        r.maximum = max(p, a.maximum);
        return r;
    }

    const AxisAlignedBoundingBox extendBy(const AxisAlignedBoundingBox & a, const Float4 & minimumIn, const Float4 & maximumIn)
    {
        AxisAlignedBoundingBox r;
        r.minimum = min(a.minimum, minimumIn);
        r.maximum = max(a.maximum, maximumIn);
        return r;
    }

    const AxisAlignedBoundingBox extendBy(const AxisAlignedBoundingBox & a, const AxisAlignedBoundingBox & b)
    {
        return extendBy(a, b.minimum, b.maximum);
    }

    const AxisAlignedBoundingBox clamp(const AxisAlignedBoundingBox & a, const AxisAlignedBoundingBox & b)
    {
        AxisAlignedBoundingBox r;
        r.minimum = clamp(a.minimum, b.minimum, b.maximum);
        r.maximum = clamp(a.maximum, b.minimum, b.maximum);
        return r;
    }

    template <ASizeT RowIndex>
    struct TransformationHelper {
        static void apply(AxisAlignedBoundingBox & out, const AxisAlignedBoundingBox & toTransform, const Float44 & m)
        {
            const Float4 f = row<RowIndex>(m);
            const Float4 a = f * toTransform.minimum, b = f * toTransform.maximum;
            out.minimum = replaceComponent<RowIndex>(
                out.minimum,
                component<RowIndex>(out.minimum + horizontalSum3v(min(a, b))));
            out.maximum = replaceComponent<RowIndex>(
                out.maximum,
                component<RowIndex>(out.maximum + horizontalSum3v(max(a, b))));
        }
    };

    const AxisAlignedBoundingBox transform(const AxisAlignedBoundingBox & a, const Float44 & m)
    {
        // Based on: Transforming Axis-Aligned Bounding Boxes by Jim Arvo (Graphics Gems, Academic Press, 1990)
		// https://github.com/erich666/GraphicsGems/blob/master/gems/TransBox.c

		// Take care of translation first
        const Float4 t = yw_yw(
            zw_zw(row<VectorIndices::X>(m), row<VectorIndices::Y>(m)),
            zw_zw(row<VectorIndices::Z>(m), One<Float4>()));

		AxisAlignedBoundingBox boxOut;
		boxOut.minimum = t;
        boxOut.maximum = t;
        // Now find the extreme points by considering the product of the
        // min and max with each row of m.
        TransformationHelper<VectorIndices::X>::apply(boxOut, a, m);
        TransformationHelper<VectorIndices::Y>::apply(boxOut, a, m);
        TransformationHelper<VectorIndices::Z>::apply(boxOut, a, m);
        return boxOut;
    }

    const bool overlaps(const AxisAlignedBoundingBox & a, const Float4 & by)
    {
        return allTrue3((a.minimum >= by) & (by >= a.maximum));
    }

    const bool overlaps(const AxisAlignedBoundingBox & a, const AxisAlignedBoundingBox & by)
    {
        return allFalse3((a.minimum > by.maximum) | (by.minimum > a.maximum));
    }

    // The near distance of a is set to "t.x". The far distance of a is set "t.y". Vice versa is set for b as "t.z" and "t.w".
    inline const Float4 computeBoxIntersectionCoefficients(const Ray & r, const AxisAlignedBoundingBox & a, const AxisAlignedBoundingBox & b)
    {
        // based on: "A Cross-Platform Framework for Interactive Ray Tracing", Geimer and Müller.
        const Float4 pMinA = (a.minimum - r.origin) * r.reciprocalDirection;
        const Float4 pMaxA = (a.maximum - r.origin) * r.reciprocalDirection;
        const Float4 pMinB = (b.minimum - r.origin) * r.reciprocalDirection;
        const Float4 pMaxB = (b.maximum - r.origin) * r.reciprocalDirection;

        const Float4 vMinA = min(pMinA, pMaxA);
        const Float4 vMaxA = max(pMinA, pMaxA);
        const Float4 vMinB = min(pMinB, pMaxB);
        const Float4 vMaxB = max(pMinB, pMaxB);

        const Float4 hA = xy_xy(vMinA, vMaxA);
        const Float4 hB = xy_xy(vMinB, vMaxB);
        const Float4 hX = xz_xz(hA, hB);
        const Float4 hY = yw_yw(hA, hB);
        const Float4 hZ = xz_xz(zw_zw(vMinA, vMaxA), zw_zw(vMinB, vMaxB));

        // x = A.t0 = hMax.x
        // y = A.t1 = hMin.y
        // z = B.t0 = hMax.z
        // w = B.t1 = hMin.w
        return blend<false, true, false, true>(
            max(hZ, max(hX, hY)),
            min(hZ, min(hX, hY)));
    }

    const Float4::VectorBoolType overlaps(const Raycast & r, const AxisAlignedBoundingBox & a, const AxisAlignedBoundingBox & b)
    {
        const Float4 c = computeBoxIntersectionCoefficients(r.ray, a, b);
        const Float4 cYW = yyww(c);
        const Float4 cXZ = xxzz(c);
        // overlaps if
        // 1. (c.y >= c.x, c.w >= c.z) and
        // 2. (c.y >= 0, c.w >= 0) and
        // 3. (c.x < 0, c.z < 0) or ray is in front and reaches the boxes
        // optimized with c & (a | !b) == !(!a & b) & c using andnot(x, y) = !x & y
        return (cYW >= cXZ) & andnot(
            andnot(cXZ < Zero<Float4>(), outOfReach(r, cXZ)),
            (cYW >= Zero<Float4>()));
    }

    // The near distance t0 is set to "t.x". The far distance t1 is set "t.y". "t.z" and "t.w" will be set to zero.
    inline const Float4 computeBoxIntersectionCoefficients(const Ray & r, const AxisAlignedBoundingBox & a)
    {
        // based on: "A Cross-Platform Framework for Interactive Ray Tracing", Geimer and Müller.
        const Float4 pMin = (a.minimum - r.origin) * r.reciprocalDirection;
        const Float4 pMax = (a.maximum - r.origin) * r.reciprocalDirection;
        const Float4 vMin = min(pMin, pMax);
        const Float4 vMax = max(pMin, pMax);
        const Float4 vXYx2 = xy_xy(vMin, vMax);
        const Float4 vXZx2 = xz_xz(vMin, vMax);
        const Float4 hMinXXYZx2 = min(vXYx2, vXZx2);
        const Float4 hMaxXXYZx2 = max(vXYx2, vXZx2);
        // x = t0, y = t1, z = w = 0
        return xz_xz(xy_zw(
            max(hMaxXXYZx2, yyww(hMaxXXYZx2)),
            min(hMinXXYZx2, yyww(hMinXXYZx2))), Zero<Float4>());
    }

    // Compares on the box intersection distances and returns the result as (x,y,z,w) as follows:
    // x = t0 < t0 = c.x < c.x = always false
    // y = t1 < t0 = c.y < c.x
    // z = t0 < 0 = c.x < c.z
    // w = t1 < 0 = c.y < c.z
    inline const Float4::VectorBoolType testBoxIntersectionCoefficients(const Float4 & c)
    {
        return xyxy(c) < xxzz(c);
    }

    const bool overlaps(const Raycast & r, const AxisAlignedBoundingBox & a)
    {
        const Float4 c = computeBoxIntersectionCoefficients(r.ray, a);
        const Float4::VectorBoolType check = testBoxIntersectionCoefficients(c);
        // no hit when t.y < t.x or t.y < 0 
        if (y(check | wwww(check)))
        {
            return false;
        }
        // if check.z is true then ray overlaps box; otherwise ray is in front and may not reach it
        // optimized with (a | !b) == !(!a & b) using andnot(x, y) = !x & y
        return !x(andnot(zzww(check), outOfReach(r, c)));
    }

    const Float nearestIntersection(const Raycast & r, const AxisAlignedBoundingBox & a, const Size2::ValueType originId)
    {
        const Float4 c = computeBoxIntersectionCoefficients(r.ray, a);
        const Float4::VectorBoolType check = testBoxIntersectionCoefficients(c);

        // no hit when c.y < c.x or c.y < 0 
        if (y(check | wwww(check)))
        {
            return r.maxDistance;
        }

        // reject c.y if backface culled
        const Float4 tt = blendMasked(yyww(c), Float4(r.maxDistance), backfaceCulledv(r));

        // reject c.x if c.x < 0 or self occluded or frontfaceCulled
        const Float4::VectorBoolType selfOccluded = convert<Float4::VectorBoolType>(selfOcclusion(r, originId));
        return x(blendMasked(c, tt, zwzw(check) | selfOccluded | frontfaceCulledv(r)));
    }

}
