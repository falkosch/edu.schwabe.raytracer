#include "../../../stdafx.h"
#include "raytracing/geometry/partitioning/KDTreeNodeChildren.h"

namespace raytracer {
  KDTreeNode::KDTreeNode() : geometryNodes(), children() {
  }

  KDTreeNode::~KDTreeNode() {
    clear();
  }

  void KDTreeNode::clear() {
    children.reset();
    geometryNodes.reset();
  }

  bool KDTreeNode::isNonEmptyLeaf() const
  {
      return geometryNodes && !geometryNodes->empty();
  }

  void KDTreeNode::grow(
      const AxisAlignedBoundingBox &leftBounding, std::unique_ptr<PGeometryNodeList> leftGeometryNodes,
      const AxisAlignedBoundingBox &rightBounding, std::unique_ptr<PGeometryNodeList> rightGeometryNodes
  ) {
    clear();
    children = std::make_unique<KDTreeNodeChildren>();
    children->boundingA = leftBounding;
    children->boundingB = rightBounding;
    children->childA.geometryNodes = std::move(leftGeometryNodes);
    children->childB.geometryNodes = std::move(rightGeometryNodes);
  }
}
