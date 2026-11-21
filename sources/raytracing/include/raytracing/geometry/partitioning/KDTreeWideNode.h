#pragma once

#include "KDTreeLink.h"
#include "KDTreeWideRay.h"

#include <primitives.h>

#include <array>
#include <cstdint>

namespace raytracer
{
    using namespace vectorization;
    using namespace primitives;

    // Node of the four-wide view of the tree: the children of an inner node's children (2 to 4), so one wide node
    // stands for two levels. Lane i of the boxes belongs to child i. Links are wide node indices, or leaf indices with
    // KDTreeRoot::LeafFlag set (the binary tree's leaves).
    struct alignas(64) KDTreeWideNode
    {
        Float4 minimumX, minimumY, minimumZ, maximumX, maximumY, maximumZ;
        std::array<KDTreeLink, 4> links;
        // bit i set for each used lane
        std::uint32_t lanes;
    };

    // Bit i set if the ray overlaps lane i's box: overlaps(rayCast, a, b) per lane, with the same operations in the
    // same order, so it decides like the binary traversal.
    unsigned overlappedLanes(const RayCast& rayCast, const KDTreeWideRay& ray, const KDTreeWideNode& node) noexcept;
    // Also sets entryDistancesOut to the distances where the ray enters the lanes' boxes (negative if it starts
    // inside).
    unsigned overlappedLanes(
        const RayCast& rayCast, const KDTreeWideRay& ray, const KDTreeWideNode& node, Float4& entryDistancesOut
    ) noexcept;
}
