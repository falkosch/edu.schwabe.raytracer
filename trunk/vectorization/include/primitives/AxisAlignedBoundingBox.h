#pragma once

#include "Raycast.h"

namespace primitives
{

    struct AxisAlignedBoundingBox
    {
        vectorization::Float4 minimum, maximum;

        ALIGNED_ALLOCATORS(__alignof(AxisAlignedBoundingBox));

        AxisAlignedBoundingBox();

        explicit AxisAlignedBoundingBox(const vectorization::Float4 & a, const vectorization::Float4 & b);
    };

    const vectorization::Float4 extents(const AxisAlignedBoundingBox & a);

    const vectorization::Float4 halfLengths(const AxisAlignedBoundingBox & a);

    const vectorization::Float4 center(const AxisAlignedBoundingBox & a);

    const vectorization::Float4 center(const AxisAlignedBoundingBox & a, const vectorization::Float4 & halfLengths);

    const vectorization::Float surfaceArea(const AxisAlignedBoundingBox & a);

    // extents = (x, y, z, 0); returns A = 2(w * d + w * h + d * h) = 2(z * y + z * x + y * x)
    const vectorization::Float surfaceArea(const AxisAlignedBoundingBox & a, const vectorization::Float4 & extents);

    const AxisAlignedBoundingBox extendBy(const AxisAlignedBoundingBox & a, const vectorization::Float4 & by);

    const AxisAlignedBoundingBox extendBy(const AxisAlignedBoundingBox & a, const vectorization::Float4 & byMin, const vectorization::Float4 & byMax);

    const AxisAlignedBoundingBox extendBy(const AxisAlignedBoundingBox & a, const AxisAlignedBoundingBox & by);

    const AxisAlignedBoundingBox clamp(const AxisAlignedBoundingBox & a, const AxisAlignedBoundingBox & by);

    const AxisAlignedBoundingBox transform(const AxisAlignedBoundingBox & a, const vectorization::Float44 & by);

    const bool overlaps(const AxisAlignedBoundingBox & a, const vectorization::Float4 & by);

    // Checks for intersection of this box with another box.
    const bool overlaps(const AxisAlignedBoundingBox & a, const AxisAlignedBoundingBox & by);

    const vectorization::Float4::VectorBoolType overlaps(const Raycast & raycast, const AxisAlignedBoundingBox & a, const AxisAlignedBoundingBox & b);

    const bool overlaps(const Raycast & raycast, const AxisAlignedBoundingBox & by);

    const vectorization::Float nearestIntersection(const Raycast & raycast, const AxisAlignedBoundingBox & a, const vectorization::Size2::ValueType originId);

}
