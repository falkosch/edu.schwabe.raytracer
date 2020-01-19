#pragma once

#include "AxisAlignedBoundingBox.h"

namespace primitives
{
    using namespace vectorization;

    struct SplittingPlane
    {
        Float4 normalDistance;

        SplittingPlane() noexcept;

        explicit SplittingPlane(const Float4 & normalDistance) noexcept;

        explicit SplittingPlane(const Float4 & origin, const Float4 & normal) noexcept;
    };

    const AxisAlignedBoundingBox bounding(const SplittingPlane & p) noexcept;

    const bool overlaps(const AxisAlignedBoundingBox & a, const SplittingPlane & p) noexcept;

    // Checks for intersection of this box with a plane.
    const bool overlaps(const Raycast & r, const SplittingPlane & by) noexcept;

    const Float nearestIntersection(const Raycast & r, const SplittingPlane & p, const Size2::ValueType originId) noexcept;
}
