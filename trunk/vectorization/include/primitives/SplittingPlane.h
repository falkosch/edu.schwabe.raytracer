#pragma once

#include "AxisAlignedBoundingBox.h"

namespace primitives
{
	using namespace vectorization;

    struct SplittingPlane
    {
        Float4 normalDistance;

        ALIGNED_ALLOCATORS(__alignof(SplittingPlane));

        SplittingPlane();

        explicit SplittingPlane(const Float4 & normalDistance);

        explicit SplittingPlane(const Float4 & origin, const Float4 & normal);
    };

    const AxisAlignedBoundingBox bounding(const SplittingPlane & p);

    const bool overlaps(const AxisAlignedBoundingBox & a, const SplittingPlane & p);

    // Checks for intersection of this box with a plane.
    const bool overlaps(const Raycast & r, const SplittingPlane & by);

    const Float nearestIntersection(const Raycast & r, const SplittingPlane & p, const Size2::ValueType originId);

}
