#include "raytracing/geometry/partitioning/KDTreeRoot.h"
#include "../../../stdafx.h"

namespace raytracer {
  KDTreeRoot::KDTreeRoot() : rootBounding(), rootNode() {
  }

  KDTreeRoot::~KDTreeRoot() = default;

  ASizeT KDTreeRoot::depth() const {
    return rootNode.depth();
  }

  ASizeT KDTreeRoot::nodeCount() const {
    return rootNode.nodeCount();
  }
}