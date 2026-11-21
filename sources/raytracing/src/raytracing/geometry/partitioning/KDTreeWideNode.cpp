#include "raytracing/geometry/partitioning/KDTreeWideNode.h"
#include "../../../stdafx.h"

namespace raytracer
{
    unsigned overlappedLanes(const RayCast& rayCast, const KDTreeWideRay& ray, const KDTreeWideNode& node) noexcept
    {
        auto entryDistances = Float4();
        return overlappedLanes(rayCast, ray, node, entryDistances);
    }

    unsigned overlappedLanes(
        const RayCast& rayCast, const KDTreeWideRay& ray, const KDTreeWideNode& node, Float4& entryDistancesOut
    ) noexcept
    {
        // computeBoxIntersectionCoefficients in AxisAlignedBoundingBox.cpp, lane-wise
        const auto minimumX = (node.minimumX - ray.originX) * ray.reciprocalX;
        const auto maximumX = (node.maximumX - ray.originX) * ray.reciprocalX;
        const auto minimumY = (node.minimumY - ray.originY) * ray.reciprocalY;
        const auto maximumY = (node.maximumY - ray.originY) * ray.reciprocalY;
        const auto minimumZ = (node.minimumZ - ray.originZ) * ray.reciprocalZ;
        const auto maximumZ = (node.maximumZ - ray.originZ) * ray.reciprocalZ;
        const auto nearX = min(minimumX, maximumX);
        const auto farX = max(minimumX, maximumX);
        const auto nearY = min(minimumY, maximumY);
        const auto farY = max(minimumY, maximumY);
        const auto nearZ = min(minimumZ, maximumZ);
        const auto farZ = max(minimumZ, maximumZ);
        const auto nearDistances = max(nearZ, max(nearX, nearY));
        const auto farDistances = min(farZ, min(farX, farY));
        entryDistancesOut = nearDistances;

        // overlaps(rayCast, a, b)
        const auto validDistances = farDistances >= nearDistances;
        const auto positiveFarDistances = farDistances >= Zero<Float4>();
        const auto negativeNearDistances = nearDistances < Zero<Float4>();
        const auto overlapping = validDistances &
            andnot(andnot(negativeNearDistances, outOfReach(rayCast, nearDistances)), positiveFarDistances);
        return static_cast<unsigned>(isNegative(overlapping)) & node.lanes;
    }
}
