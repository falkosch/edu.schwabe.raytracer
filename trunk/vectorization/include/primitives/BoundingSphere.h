#pragma once

#include "AxisAlignedBoundingBox.h"

namespace primitives
{

    struct BoundingSphere
    {
        // contains the XYZ-coords of the sphere's center and the radius as the w-component
        vectorization::Float4 centerRadius;

        ALIGNED_ALLOCATORS(__alignof(BoundingSphere));

        BoundingSphere();

        explicit BoundingSphere(const vectorization::Float4 & center, const vectorization::Float radius);
    };

    const AxisAlignedBoundingBox bounding(const BoundingSphere & b);

    const bool overlaps(const Raycast & raycast, const BoundingSphere & by);

    const vectorization::Float nearestIntersection(const Raycast & r, const BoundingSphere & b, const vectorization::Size2::ValueType originId);

}
