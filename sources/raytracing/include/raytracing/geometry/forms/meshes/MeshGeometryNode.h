#pragma once

#include "../../partitioning/GeometryNode.h"

#include <primitives.h>

namespace raytracer {
  using namespace vectorization;
  using namespace primitives;

  struct MeshGeometryNode : GeometryNode {
    ASizeT index;

    Facet facet;

    MeshGeometryNode();

    explicit MeshGeometryNode(ASizeT index, const Facet &facet);

    // GeometryNode interface

    AxisAlignedBoundingBox includeInBounding(const AxisAlignedBoundingBox &aabb) const;

    bool overlaps(const AxisAlignedBoundingBox &aabb) const override;

    Float getIndividualIntersectionCosts() const override {
      return 79.1726f;
    }

    // Intersectable<RayCast, FacetIntersection> interface

    Float findNearestIntersection(
        const RayCast &rayCast, const FacetIntersection *originIntersection, FacetIntersection &intersectionOut
    ) const override;

    Float findAnyIntersection(
        const RayCast &rayCast, const FacetIntersection *originIntersection, FacetIntersection &intersectionOut
    ) const override;
  };
}
