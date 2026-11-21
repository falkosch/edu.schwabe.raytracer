#pragma once

#include <cstdint>

namespace raytracer
{
    // Reference to a node of the flat KD-tree: the index of an inner node, or the index of a leaf with
    // KDTreeRoot::LeafFlag set.
    using KDTreeLink = std::uint32_t;
}
