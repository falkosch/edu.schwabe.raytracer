#pragma once

#include "KDTreeBounding.h"

namespace raytracer {
  using namespace primitives;

  struct KDTreeNodeChildren;

  struct KDTreeNode {
    PGeometryNodeList *geometryNodes;

    KDTreeNodeChildren *children;

    KDTreeNode();

    ~KDTreeNode();

    void clear();

    const bool isNonEmptyLeaf() const;

    // Adds two new children to this node and makes this node an empty node by deleting the geometryNodes.
    void grow(
        const AxisAlignedBoundingBox &leftBounding, PGeometryNodeList &leftGeometryNodes,
        const AxisAlignedBoundingBox &rightBounding, PGeometryNodeList &rightGeometryNodes
    );
  };
}
