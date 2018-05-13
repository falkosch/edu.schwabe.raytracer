#pragma once

#include "geometry/forms/Form.h"

#include <primitives/BoundingSphere.h>

namespace raytracer
{

    struct Sphere : public Form
    {
        BoundingSphere bounding;

        ALIGNED_ALLOCATORS(__alignof(Sphere));

        Sphere();

        explicit Sphere(const BoundingSphere & bounding);

        explicit Sphere(const Float4 & center, const Float radius);

        virtual ~Sphere();

        const AxisAlignedBoundingBox getBounding() const;

        const Float findNearestIntersection(const Raycast & r, const FacetIntersection * const originIntersection, FacetIntersection & intersectionOut) const;

        const Float findAnyIntersection(const Raycast & r, const FacetIntersection * const originIntersection, FacetIntersection & intersectionOut) const;

        const Float getIndividualIntersectionCosts() const
        {
            // benchmarked intersection costs in cycles
            return 37.8098f;
        }
    };

}
