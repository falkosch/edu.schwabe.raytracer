#pragma once

#include "GeometryNode.h"

namespace raytracer {
  using namespace vectorization;
  using namespace primitives;

  template <typename IntersectionInfoType>
  class GeometryNodesTraverser {
  public:
    virtual ~GeometryNodesTraverser() {
    }

    // Finds the nearest intersection of a Ray within a geometry.
    virtual Float findNearestIntersection(
        const PGeometryNodeList &geometryNodes, const RayCast &rayCast, const IntersectionInfoType *originIntersection,
        IntersectionInfoType &intersectionOut
    ) const = 0;

    // Finds any intersection of a Ray within a geometry.
    virtual Float findAnyIntersection(
        const PGeometryNodeList &geometryNodes, const RayCast &rayCast, const IntersectionInfoType *originIntersection,
        IntersectionInfoType &intersectionOut
    ) const = 0;
  };
}
