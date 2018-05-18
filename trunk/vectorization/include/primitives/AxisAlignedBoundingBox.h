#pragma once

#include "Raycast.h"

namespace primitives
{
	using namespace vectorization;

    struct AxisAlignedBoundingBox
    {
        Float4 minimum, maximum;

        ALIGNED_ALLOCATORS(__alignof(AxisAlignedBoundingBox));

        AxisAlignedBoundingBox();

        explicit AxisAlignedBoundingBox(const Float4 & a, const Float4 & b);
    };

    const Float4 extents(const AxisAlignedBoundingBox & a);

    const Float4 halfLengths(const AxisAlignedBoundingBox & a);

    const Float4 center(const AxisAlignedBoundingBox & a);

    const Float4 center(const AxisAlignedBoundingBox & a, const Float4 & halfLengths);

    const Float surfaceArea(const AxisAlignedBoundingBox & a);

    // extents = (x, y, z, 0); returns A = 2(w * d + w * h + d * h) = 2(z * y + z * x + y * x)
    const Float surfaceArea(const AxisAlignedBoundingBox & a, const Float4 & extents);

    const AxisAlignedBoundingBox extendBy(const AxisAlignedBoundingBox & a, const Float4 & by);

    const AxisAlignedBoundingBox extendBy(const AxisAlignedBoundingBox & a, const Float4 & byMin, const Float4 & byMax);

    const AxisAlignedBoundingBox extendBy(const AxisAlignedBoundingBox & a, const AxisAlignedBoundingBox & by);

    const AxisAlignedBoundingBox clamp(const AxisAlignedBoundingBox & a, const AxisAlignedBoundingBox & by);

    const AxisAlignedBoundingBox transform(const AxisAlignedBoundingBox & a, const Float44 & by);

    const bool overlaps(const AxisAlignedBoundingBox & a, const Float4 & by);

    // Checks for intersection of this box with another box.
    const bool overlaps(const AxisAlignedBoundingBox & a, const AxisAlignedBoundingBox & by);

    const Float4::VectorBoolType overlaps(const Raycast & raycast, const AxisAlignedBoundingBox & a, const AxisAlignedBoundingBox & b);

    const bool overlaps(const Raycast & raycast, const AxisAlignedBoundingBox & by);

    const Float nearestIntersection(const Raycast & raycast, const AxisAlignedBoundingBox & a, const Size2::ValueType originId);

}
