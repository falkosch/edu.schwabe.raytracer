#pragma once

#include "GeometryNodesTraverser.h"
#include "KDTreeRoot.h"

namespace raytracer {
  using namespace primitives;

  template <typename IntersectionInfoType>
  class KDTreeTraverser {
  public:
    virtual ~KDTreeTraverser() {
    }

    virtual const Float findNearestIntersection(
        const GeometryNodesTraverser<IntersectionInfoType> &geometryNodesTraverser, const KDTreeRoot &root,
        const RayCast &rayCast, const IntersectionInfoType *const originIntersection,
        IntersectionInfoType &intersectionOut
    ) const = 0;

    virtual const Float findAnyIntersection(
        const GeometryNodesTraverser<IntersectionInfoType> &geometryNodesTraverser, const KDTreeRoot &root,
        const RayCast &rayCast, const IntersectionInfoType *const originIntersection,
        IntersectionInfoType &intersectionOut
    ) const = 0;
  };
}
