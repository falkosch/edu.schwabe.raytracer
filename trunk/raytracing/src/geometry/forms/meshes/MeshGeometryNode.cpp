#include "../../../stdafx.h"

#include "geometry/forms/meshes/MeshGeometryNode.h"

namespace raytracer
{

#pragma region privates
    //{privates

    const Float4::VectorBoolType planeBoxOverlap(const Float4 & normal, const Float4 & vert, const Float4 & maxbox)
    {
        const Float4::VectorBoolType mask = (normal > Zero<Float4>());
        const Float4 a = maxbox - vert, b = -maxbox - vert;
        const Float4::VectorBoolType positiveSide = dot3v(normal, blendMasked(a, b, mask)) <= Zero<Float4>();
        const Float4::VectorBoolType negativeSide = dot3v(normal, blendMasked(b, a, mask)) >= Zero<Float4>();
        return positiveSide & negativeSide;
    }

    const bool axisTest(
        const Float4 & edge,
        const Float4 & yzxBoxHalfSize,
        const Float4 & zxyBoxHalfSize,
        const Float4 & w00,
        const Float4 & w01,
        const Float4 & w10,
        const Float4 & w11)
    {
        const Float4 fe = abs(edge);
        const Float4 rad = zxyw(fe) * yzxBoxHalfSize + yzxw(fe) * zxyBoxHalfSize;
        const Float4 zxyE = zxyw(edge), yzxE = yzxw(edge);
        const Float4 p135 = zxyE * w00 - yzxE * w01;
        const Float4 p246 = zxyE * w10 - yzxE * w11;
        return anyTrue3(min(p135, p246) > rad | max(p135, p246) < -rad);
    }

    const bool triBoxOverlap(const Float4 & boxCenter, const Float4 & boxHalfSize, const Facet & trianglePlanes)
    {
        const Float4 v0 = trianglePlanes.v0 - boxCenter, v1 = trianglePlanes.v1 - boxCenter, v2 = trianglePlanes.v2 - boxCenter;
        const Float4 yzxBoxHalfSize = yzxw(boxHalfSize);
        const Float4 zxyBoxHalfSize = zxyw(boxHalfSize);
        const Float4 e0 = v1 - v0, e1 = v2 - v1;
        const Float4 yzxV0 = yzxw(v0), zxyV0 = zxyw(v0);
        const Float4 yyyV1 = yyyy(v1);
        const Float4 yzxV2 = yzxw(v2), zxyV2 = zxyw(v2);

        if (axisTest(e0, yzxBoxHalfSize, zxyBoxHalfSize,
            xy_xy(yzxV0, v1), xy_xy(zxyV0, yyyV1),
            yzxV2, zxyV2
        )) return false;

        if (axisTest(e1, yzxBoxHalfSize, zxyBoxHalfSize,
            yzxV0, zxyV0,
            xy_xy(yzxV2, v1), xy_xy(zxyV2, yyyV1)
        )) return false;

        if (axisTest(v0 - v2, yzxBoxHalfSize, zxyBoxHalfSize,
            xy_xy(yzxV0, v1), xy_xy(zxyV0, yyyV1),
            xy_xy(yzxw(v1), v2), xy_xy(zxyw(v1), yyyy(v2))
        )) return false;

        if (anyTrue3((min(v0, min(v1, v2)) > boxHalfSize) | (max(v0, max(v1, v2)) < -boxHalfSize))) return false;

        return allTrue(planeBoxOverlap(cross3(e0, e1), v0, boxHalfSize));
    }

    //}
#pragma endregion

    MeshGeometryNode::MeshGeometryNode()
        :
        index(),
        trianglePlanes()
    { }

    MeshGeometryNode::MeshGeometryNode(const ASizeT index, const Facet & trianglePlanes)
        :
        index(index),
        trianglePlanes(trianglePlanes)
    { }

    // GeometryNode interface

    void MeshGeometryNode::includeInBounding(AxisAlignedBoundingBox & aabb) const
    {
        aabb = extendBy(extendBy(
            extendBy(aabb, trianglePlanes.v0), trianglePlanes.v1),
            trianglePlanes.v2);
    }

    const bool MeshGeometryNode::overlaps(const AxisAlignedBoundingBox & aabb) const
    {
        const Float4 boxHalfSize = halfLengths(aabb);
        return triBoxOverlap(center(aabb, boxHalfSize), boxHalfSize, trianglePlanes);
    }

    // Intersectable<Raycast, FacetIntersection> interface

    const Float MeshGeometryNode::findNearestIntersection(const Raycast & ray, const FacetIntersection * const originIntersection, FacetIntersection & intersectionOut) const
    {
        return ray.maxDistance;
    }

    const Float MeshGeometryNode::findAnyIntersection(const Raycast & ray, const FacetIntersection * const originIntersection, FacetIntersection & intersectionOut) const
    {
        return ray.maxDistance;
    }
}
