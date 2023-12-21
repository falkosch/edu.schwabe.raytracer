#include "raytracing/geometry/partitioning/treebalancer/MaxAxisKDTreeBalancer.h"
#include "../../../../stdafx.h"

namespace raytracer {
  MaxAxisKDTreeBalancer::MaxAxisKDTreeBalancer() : KDTreeBalancer() {
  }

  MaxAxisKDTreeBalancer::~MaxAxisKDTreeBalancer() = default;

  const KDTreePlane MaxAxisKDTreeBalancer::
      findSplitter(const KDTreeBuildParameters &, const ASizeT, const AxisAlignedBoundingBox &bounding, const PGeometryNodeList &, const KDTreePlane *)
          const {
    return KDTreePlane(Half<Float>(), argmax3(extents(bounding)));
  }
}