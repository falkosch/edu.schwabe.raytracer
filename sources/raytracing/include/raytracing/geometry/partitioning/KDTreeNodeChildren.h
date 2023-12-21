#pragma once

#include "KDTreeNode.h"

namespace raytracer {
  using namespace primitives;

  // KD-trees have a very special structure: Non-leaf-nodes always have two
  // child nodes. That is why we use a dedicated KDTreeNodeChildren structure
  // that reflects that inherent property.
  struct KDTreeNodeChildren {
    AxisAlignedBoundingBox boundingA, boundingB;

    // the two children
    KDTreeNode childA, childB;

    KDTreeNodeChildren();

    ~KDTreeNodeChildren();
  };
}
