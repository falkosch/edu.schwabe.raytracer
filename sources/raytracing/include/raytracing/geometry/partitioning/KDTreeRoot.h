#pragma once

#include "KDTreeNodeChildren.h"

namespace raytracer {
  using namespace primitives;

  struct KDTreeRoot {
    AxisAlignedBoundingBox rootBounding;

    KDTreeNode rootNode;

    KDTreeRoot();

    ~KDTreeRoot();
  };
}
