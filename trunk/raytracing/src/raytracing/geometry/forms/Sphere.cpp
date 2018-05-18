#include "raytracing/geometry/forms/Sphere.h"
#include "../../../stdafx.h"

#include "raytracing/common/Tools.h"

namespace raytracer
{

    Sphere::Sphere()
        :
        bounding(OneW<Float4>(), One<Float>())
    { }

    Sphere::Sphere(const Float4 & center, const Float radius)
        :
        bounding(center, radius)
    {
        assert(radius > Zero<Float>());
    }

    Sphere::~Sphere() { }

    const AxisAlignedBoundingBox Sphere::getBounding() const
    {
        return primitives::bounding(bounding);
    }

    const Float computeFacetIntersection(const Float d, const Raycast & raycast, const Sphere & s, FacetIntersection & intersectionOut)
    {
        if (outOfReach(raycast, d))
        {
            return raycast.maxDistance;
        }
        const Float4 msVertex = point(raycast.ray, d);
        const Float4 normal = normalize3(msVertex - s.bounding.centerRadius);
        intersectionOut.msVertex = msVertex;
        intersectionOut.msSurfaceNormal = normal;
        intersectionOut.vertex = msVertex;
        intersectionOut.surfaceNormal = normal;
        intersectionOut.smoothedNormal = normal;
        intersectionOut.texCoords = mapSpherical(normal);
        intersectionOut.node = &s;
        return d;
    }

    const Float Sphere::findNearestIntersection(const Raycast & r, const FacetIntersection * const, FacetIntersection & intersectionOut) const
    {
        return computeFacetIntersection(
            nearestIntersection(r, bounding, reinterpret_cast<ASizeT>(this)),
            r,
            *this,
            intersectionOut);
    }

    const Float Sphere::findAnyIntersection(const Raycast & r, const FacetIntersection * const, FacetIntersection & intersectionOut) const
    {
        return computeFacetIntersection(
            nearestIntersection(r, bounding, reinterpret_cast<ASizeT>(this)),
            r,
            *this,
            intersectionOut);
    }

}
