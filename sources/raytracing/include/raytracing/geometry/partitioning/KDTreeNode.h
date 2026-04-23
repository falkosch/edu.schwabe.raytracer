#pragma once

#include "KDTreeBounding.h"

#include <memory>

namespace raytracer {
  using namespace primitives;

  struct KDTreeNodeChildren;

  struct KDTreeNode {
    std::unique_ptr<PGeometryNodeList> geometryNodes;

    std::unique_ptr<KDTreeNodeChildren> children;

    KDTreeNode();

    ~KDTreeNode();

    void clear();

    bool isNonEmptyLeaf() const;

    // Adds two new children to this node and makes this node an empty node by deleting the geometryNodes.
    void grow(
        const AxisAlignedBoundingBox &leftBounding, std::unique_ptr<PGeometryNodeList> leftGeometryNodes,
        const AxisAlignedBoundingBox &rightBounding, std::unique_ptr<PGeometryNodeList> rightGeometryNodes
    );
  };
}
