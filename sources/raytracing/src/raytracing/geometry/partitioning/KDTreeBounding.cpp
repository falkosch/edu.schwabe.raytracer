#include "raytracing/geometry/partitioning/KDTreeBounding.h"
#include "../../../stdafx.h"

namespace raytracer {
  void KDTreeBounding::split(
      const KDTreePlane &splitter, const AxisAlignedBoundingBox &toSplit, AxisAlignedBoundingBox &leftBounding,
      AxisAlignedBoundingBox &rightBounding
  ) {
    leftBounding = AxisAlignedBoundingBox(toSplit.minimum, splitter.splitMax(toSplit.minimum, toSplit.maximum));
    rightBounding = AxisAlignedBoundingBox(splitter.splitMin(toSplit.minimum, toSplit.maximum), toSplit.maximum);
  }

  const AxisAlignedBoundingBox KDTreeBounding::findMinimumBoundingOfGeometry(const PGeometryNodeList &geometry) {
    const int geometrySize = static_cast<int>(geometry.size());
    AxisAlignedBoundingBox bounding = AxisAlignedBoundingBox();

#pragma omp parallel if (geometrySize >= 64)
    {
      AxisAlignedBoundingBox t = AxisAlignedBoundingBox();

#pragma omp for nowait
      for (int i = Zero<int>(); i < geometrySize; ++i) {
        t = geometry[i]->includeInBounding(t);
      }

#pragma omp critical(mergeBounding)
      { bounding = extendBy(t, bounding); }
    }

    return bounding;
  }
}
