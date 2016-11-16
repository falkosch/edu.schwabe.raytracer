#pragma once

#include "geometry/forms/Form.h"

#include <primitives/SplittingPlane.h>
#include <primitives/FacetEdges.h>

namespace raytracer
{

    class Plane : public Form
    {
        void reconstructFacetEdgesFromPlaneEquation(const Float4 & planeEqu);

    public:

        SplittingPlane plane;

        FacetEdges baseVectors;

        ALIGNED_ALLOCATORS(__alignof(Plane));

        Plane();

        explicit Plane(const Float4 & planeEquation);

        explicit Plane(const Float4 & origin, const Float4 & normal);

        explicit Plane(const Float4 & origin, const FacetEdges & baseVectors);

        virtual ~Plane();

        const bool isInfinite() const;

        const Float findNearestIntersection(const Raycast & raycast, const FacetIntersection * const originIntersection, FacetIntersection & intersectionOut) const;

        const Float findAnyIntersection(const Raycast & raycast, const FacetIntersection * const originIntersection, FacetIntersection & intersectionOut) const;

        const Float getIndividualIntersectionCosts() const
        {
            // benchmarked intersection costs in cycles
            return 26.219916f;
        }
    };

}
