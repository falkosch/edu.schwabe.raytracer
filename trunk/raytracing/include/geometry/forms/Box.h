#pragma once

#include "geometry/forms/Form.h"

#include <primitives/AxisAlignedBoundingBox.h>

namespace raytracer
{

    struct Box : public Form
    {
        AxisAlignedBoundingBox bounding;

        ALIGNED_ALLOCATORS(__alignof(Box));

        Box();

        explicit Box(const AxisAlignedBoundingBox & bounding);

        explicit Box(const Float4 & min, const Float4 & max);

        virtual ~Box();

        const AxisAlignedBoundingBox getBounding() const;

        const Float findNearestIntersection(const Raycast & raycast, const FacetIntersection * const originIntersection, FacetIntersection & intersectionOut) const;

        const Float findAnyIntersection(const Raycast & raycast, const FacetIntersection * const originIntersection, FacetIntersection & intersectionOut) const;

        const Float getIndividualIntersectionCosts() const
        {
            // benchmarked intersection costs in cycles
            return 26.003124f;
        }
    };

}
