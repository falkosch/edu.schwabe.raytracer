#pragma once

#include <vectorization/vectorization.h>

namespace raytracer
{

    template <typename RayType, typename IntersectionType>
    struct Intersectable
    {
        virtual ~Intersectable() { }

        virtual const Float findNearestIntersection(
            const RayType & ray,
            const IntersectionType * const originIntersection,
            IntersectionType & intersectionOut) const = 0;

        virtual const Float findAnyIntersection(
            const RayType & ray,
            const IntersectionType * const originIntersection,
            IntersectionType & intersectionOut) const = 0;
    };

}
