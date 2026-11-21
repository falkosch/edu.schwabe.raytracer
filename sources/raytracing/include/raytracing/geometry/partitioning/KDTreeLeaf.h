#pragma once

#include <cstdint>

namespace raytracer
{
    // A leaf's range in KDTreeRoot::leafGeometry.
    struct KDTreeLeaf
    {
        std::uint32_t offset, count;
    };
}
