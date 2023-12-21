#pragma once

#include "../KDTreeBalancer.h"

namespace raytracer {
  using namespace vectorization;
  using namespace primitives;

  class MaxAxisKDTreeBalancer : public KDTreeBalancer {
  public:
    MaxAxisKDTreeBalancer();

    virtual ~MaxAxisKDTreeBalancer();

    const KDTreePlane findSplitter(
        const KDTreeBuildParameters &parameters, const ASizeT treeDepth, const AxisAlignedBoundingBox &bounding,
        const PGeometryNodeList &geometry, const KDTreePlane *parentSplitter
    ) const;
  };
}
