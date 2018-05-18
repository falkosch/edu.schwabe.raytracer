#pragma once

#include "AxisAlignedBoundingBox.h"

namespace primitives
{
	using namespace vectorization;

    struct BoundingSphere
    {
        // contains the XYZ-coords of the sphere's center and the radius as the w-component
        Float4 centerRadius;

        ALIGNED_ALLOCATORS(__alignof(BoundingSphere));

        BoundingSphere();

        explicit BoundingSphere(const Float4 & center, const Float radius);
    };

    const AxisAlignedBoundingBox bounding(const BoundingSphere & b);

    const bool overlaps(const Raycast & raycast, const BoundingSphere & by);

    const Float nearestIntersection(const Raycast & r, const BoundingSphere & b, const Size2::ValueType originId);

}
