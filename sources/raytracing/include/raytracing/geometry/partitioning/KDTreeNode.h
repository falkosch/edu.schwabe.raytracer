#pragma once

#include "KDTreeBounding.h"

namespace raytracer
{
    using namespace primitives;

    struct KDTreeNodeChilds;

    struct KDTreeNode
    {
        PGeometryNodeList * geometryNodes;

        KDTreeNodeChilds * childs;

        KDTreeNode();

        ~KDTreeNode();

        void clear();

        const bool isNonEmptyLeaf() const;

        // Adds two new childs to this node and makes this node an empty node by deleting the geometryNodes.
        void grow(
            const AxisAlignedBoundingBox & leftBounding,
            PGeometryNodeList & leftGeometryNodes,
            const AxisAlignedBoundingBox & rightBounding,
            PGeometryNodeList & rightGeometryNodes
        );
    };
}
