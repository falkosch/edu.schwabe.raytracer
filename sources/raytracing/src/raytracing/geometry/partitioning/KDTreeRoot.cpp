#include "raytracing/geometry/partitioning/KDTreeRoot.h"
#include "../../../stdafx.h"

#include <cassert>

namespace raytracer
{
    static_assert(
        sizeof(KDTreeChildBoundings) == 2 * sizeof(AxisAlignedBoundingBox),
        "both child boundings must fill KDTreeChildBoundings without padding"
    );

    KDTreeRoot::KDTreeRoot()
        : rootBounding(), rootLink(LeafFlag), childBoundings(), childLinks(), leaves(), leafGeometry(), treeDepth()
    {
    }

    KDTreeRoot::~KDTreeRoot() = default;

    bool KDTreeRoot::isLeaf(const KDTreeLink link) noexcept
    {
        return (link & LeafFlag) != 0;
    }

    GeometryNodeSpan KDTreeRoot::geometryOf(const KDTreeLink leafLink) const noexcept
    {
        assert(isLeaf(leafLink));
        const auto& leaf = leaves[leafLink & ~LeafFlag];
        return GeometryNodeSpan(leafGeometry.data() + leaf.offset, leaf.count);
    }

    ASizeT KDTreeRoot::depth() const
    {
        return treeDepth;
    }

    ASizeT KDTreeRoot::nodeCount() const
    {
        return childLinks.size() + leaves.size();
    }
}
