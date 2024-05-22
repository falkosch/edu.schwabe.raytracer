#include "raytracing/geometry/partitioning/treebalancer/SAHKDTreeBalancer.h"
#include "../../../../stdafx.h"

namespace raytracer {
  SAHKDTreeBalancer::~SAHKDTreeBalancer() = default;

  void SAHKDTreeBalancer::testSplit(
      const KDTreeBuildParameters &parameters, const Float4 &position, const ASizeT axis,
      const PGeometryNodeList &geometry, const AxisAlignedBoundingBox &parentBounding, Float &bestCost,
      KDTreePlane &bestPlane
  ) const {
    const Float splitCoordOnAxis = position[axis];
    const KDTreePlane splitPlane = SamplingKDTreeBalancer::sampleSplittingPlane(axis, parentBounding, position);

    AxisAlignedBoundingBox leftBounding, rightBounding;
    KDTreeBounding::split(splitPlane, parentBounding, leftBounding, rightBounding);

    PGeometryNodeList leftGeometry = PGeometryNodeList();
    std::copy_if(geometry.cbegin(), geometry.cend(), std::back_inserter(leftGeometry), [=](auto geometryNode) {
      return SAHKDTreeBalancer::geometryNodeBounding(*geometryNode).minimum[axis] <= splitCoordOnAxis;
    });

    leftBounding = clamp(leftBounding, KDTreeBounding::findMinimumBoundingOfGeometry(leftGeometry));
    if (isTerminal(parameters, Zero<ASizeT>(), leftBounding, leftGeometry)) {
      return;
    }

    PGeometryNodeList rightGeometry = PGeometryNodeList();
    std::copy_if(geometry.cbegin(), geometry.cend(), std::back_inserter(rightGeometry), [=](auto geometryNode) {
      return SAHKDTreeBalancer::geometryNodeBounding(*geometryNode).maximum[axis] >= splitCoordOnAxis;
    });

    rightBounding = clamp(rightBounding, KDTreeBounding::findMinimumBoundingOfGeometry(rightGeometry));
    if (isTerminal(parameters, Zero<ASizeT>(), rightBounding, rightGeometry)) {
      return;
    }

    const Float splitCost =
        calculateSplitCost(parameters, parentBounding, leftBounding, rightBounding, leftGeometry, rightGeometry);

    const bool isBetter = splitCost < bestCost;
    bestCost = select(isBetter, splitCost, bestCost);
    bestPlane = select(isBetter, splitPlane, bestPlane);
  }

  const AxisAlignedBoundingBox SAHKDTreeBalancer::geometryNodeBounding(const GeometryNode &node) {
    return node.includeInBounding(AxisAlignedBoundingBox());
  }
}
