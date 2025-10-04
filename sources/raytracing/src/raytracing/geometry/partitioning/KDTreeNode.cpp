#include "../../../stdafx.h"
#include "raytracing/geometry/partitioning/KDTreeNodeChildren.h"

namespace raytracer {
  KDTreeNode::KDTreeNode() : geometryNodes(), children() {
  }

  KDTreeNode::~KDTreeNode() {
    clear();
  }

  void KDTreeNode::clear() {
    if (children) {
      delete children;
      children = nullptr;
    }
    if (geometryNodes) {
      delete geometryNodes;
      geometryNodes = nullptr;
    }
  }

  const bool KDTreeNode::isNonEmptyLeaf() const {
    return geometryNodes && !geometryNodes->empty();
  }

  void KDTreeNode::grow(
      const AxisAlignedBoundingBox &leftBounding, PGeometryNodeList &leftGeometryNodes,
      const AxisAlignedBoundingBox &rightBounding, PGeometryNodeList &rightGeometryNodes
  ) {
    clear();
    children = new KDTreeNodeChildren();
    children->boundingA = leftBounding;
    children->boundingB = rightBounding;
    children->childA.geometryNodes = &leftGeometryNodes;
    children->childB.geometryNodes = &rightGeometryNodes;
  }
}
