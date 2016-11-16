#pragma once

#include "AxisAlignedBoundingBox.h"

namespace primitives
{

    struct SplittingPlane
    {
        vectorization::Float4 normalDistance;

        ALIGNED_ALLOCATORS(__alignof(SplittingPlane));

        SplittingPlane();

        explicit SplittingPlane(const vectorization::Float4 & normalDistance);

        explicit SplittingPlane(const vectorization::Float4 & origin, const vectorization::Float4 & normal);
    };

    const AxisAlignedBoundingBox bounding(const SplittingPlane & p);

    const bool overlaps(const AxisAlignedBoundingBox & a, const SplittingPlane & p);

    // Checks for intersection of this box with a plane.
    const bool overlaps(const Raycast & r, const SplittingPlane & by);

    const vectorization::Float nearestIntersection(const Raycast & r, const SplittingPlane & p, const vectorization::Size2::ValueType originId);

}
