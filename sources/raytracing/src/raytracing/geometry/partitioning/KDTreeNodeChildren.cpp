#include "raytracing/geometry/partitioning/KDTreeNodeChildren.h"
#include "../../../stdafx.h"

namespace raytracer {
  KDTreeNodeChildren::KDTreeNodeChildren() : boundingA(), boundingB(), childA(), childB() {
  }

  KDTreeNodeChildren::~KDTreeNodeChildren() = default;
}
