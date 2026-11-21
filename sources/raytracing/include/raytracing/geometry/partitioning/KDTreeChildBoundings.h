#pragma once

#include <primitives.h>

namespace raytracer
{
    using namespace primitives;

    // The boundings of both children of an inner node, tested together by the traversal. In single precision that is
    // 64 bytes, one cache line.
    struct alignas(64) KDTreeChildBoundings
    {
        AxisAlignedBoundingBox boundingA, boundingB;
    };

    static_assert(
        sizeof(KDTreeChildBoundings) == 2 * sizeof(AxisAlignedBoundingBox),
        "both child boundings must fill KDTreeChildBoundings without padding"
    );
}
