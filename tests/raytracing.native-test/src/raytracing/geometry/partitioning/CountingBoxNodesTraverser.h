#pragma once

#include "BoxNodesTraverser.h"

namespace raytracer::test {
  // Counts the leaves the traversal hands over, then intersects like BoxNodesTraverser.
  class CountingBoxNodesTraverser : public BoxNodesTraverser {
  public:
    mutable int leafCalls = 0;

    Float findNearestIntersection(
        GeometryNodeSpan geometryNodes, const RayCast &rayCast, const FacetIntersection *const origin,
        FacetIntersection &intersectionOut
    ) const override {
      ++leafCalls;
      return BoxNodesTraverser::findNearestIntersection(geometryNodes, rayCast, origin, intersectionOut);
    }

    Float findAnyIntersection(
        GeometryNodeSpan geometryNodes, const RayCast &rayCast, const FacetIntersection *const origin,
        FacetIntersection &intersectionOut
    ) const override {
      ++leafCalls;
      return BoxNodesTraverser::findAnyIntersection(geometryNodes, rayCast, origin, intersectionOut);
    }
  };
}
