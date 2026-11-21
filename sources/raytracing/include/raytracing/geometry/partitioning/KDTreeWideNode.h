#pragma once

#include "KDTreeLink.h"
#include "KDTreeWideLanes.h"
#include "KDTreeWideRay.h"

#include <primitives.h>

#include <array>
#include <bit>
#include <cstdint>

namespace raytracer
{
    using namespace vectorization;
    using namespace primitives;

    // Node of the wide view of the tree: the descendants of an inner node Levels levels down (2 to Width of them; a
    // leaf above that level takes one lane), so one wide node stands for Levels levels. Lanes are in depth-first order,
    // child A before child B. Lane i of the boxes belongs to descendant i. Links are wide node indices, or leaf indices
    // with KDTreeRoot::LeafFlag set (the binary tree's leaves).
    struct alignas(64) KDTreeWideNode
    {
        static constexpr ASizeT Width = KDTreeWideLanes::SIZE;
        static constexpr ASizeT Levels = std::bit_width(Width) - 1;

        KDTreeWideLanes minimumX, minimumY, minimumZ, maximumX, maximumY, maximumZ;
        std::array<KDTreeLink, Width> links;
        // bit i set for each used lane
        std::uint32_t lanes;
    };

    // Bit i set if the ray overlaps lane i's box: overlaps(rayCast, a, b) per lane, with the same operations in the
    // same order, so it decides like the binary traversal.
    unsigned overlappedLanes(const RayCast& rayCast, const KDTreeWideRay& ray, const KDTreeWideNode& node) noexcept;
    // Also sets entryDistancesOut to the distances where the ray enters the lanes' boxes (negative if it starts
    // inside).
    unsigned overlappedLanes(
        const RayCast& rayCast, const KDTreeWideRay& ray, const KDTreeWideNode& node, KDTreeWideLanes& entryDistancesOut
    ) noexcept;
}
