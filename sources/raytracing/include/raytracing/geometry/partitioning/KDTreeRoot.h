#pragma once

#include "GeometryNode.h"

#include <cstdint>
#include <vector>

namespace raytracer
{
    using namespace primitives;

    // Reference to a node of the flat KD-tree: the index of an inner node, or the index of a leaf with
    // KDTreeRoot::LeafFlag set.
    using KDTreeLink = std::uint32_t;

    // The boundings of both children of an inner node, tested together by the traversal. In single precision that is
    // 64 bytes, one cache line.
    struct alignas(64) KDTreeChildBoundings
    {
        AxisAlignedBoundingBox boundingA, boundingB;
    };

    struct KDTreeChildLinks
    {
        KDTreeLink childA, childB;
    };

    // A leaf's range in KDTreeRoot::leafGeometry.
    struct KDTreeLeaf
    {
        std::uint32_t offset, count;
    };

    // KD-tree in flat arrays. Inner nodes are stored depth-first (child A right after its parent), children and leaves
    // are referenced by index. Built by KDTreeBalancer::build().
    struct KDTreeRoot
    {
        static constexpr KDTreeLink LeafFlag = KDTreeLink{1} << 31;

        // upper bound for depth(), sizes the traversal stacks
        static constexpr ASizeT MaxDepth = 64;

        AxisAlignedBoundingBox rootBounding;

        // inner node 0, or a leaf if the whole tree is a single leaf
        KDTreeLink rootLink;

        // per inner node, same index in both
        std::vector<KDTreeChildBoundings> childBoundings;
        std::vector<KDTreeChildLinks> childLinks;

        std::vector<KDTreeLeaf> leaves;

        // the geometry of all leaves back to back; geometry straddling a split occurs once per leaf
        PGeometryNodeList leafGeometry;

        // longest path from the root to a leaf, counted in nodes
        ASizeT treeDepth;

        KDTreeRoot();

        ~KDTreeRoot();

        static bool isLeaf(KDTreeLink link) noexcept;

        // geometry of the leaf the link points to
        GeometryNodeSpan geometryOf(KDTreeLink leafLink) const noexcept;

        ASizeT depth() const;

        // inner nodes plus leaves
        ASizeT nodeCount() const;
    };
}
