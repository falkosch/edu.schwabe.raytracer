#pragma once

#include "../FacetIntersection.h"

#include <primitives.h>

#include <vector>

namespace raytracer {
  using namespace vectorization;
  using namespace primitives;

  struct GeometryNode : Intersectable<RayCast, FacetIntersection> {
    ~GeometryNode() override;

    virtual AxisAlignedBoundingBox includeInBounding(const AxisAlignedBoundingBox &aabb) const = 0;

    virtual bool overlaps(const AxisAlignedBoundingBox &aabb) const = 0;

    // benchmarked intersection costs in cpu cycles
    virtual Float getIndividualIntersectionCosts() const = 0;
  };

  typedef std::vector<GeometryNode *, AlignedAllocator<GeometryNode *>> PGeometryNodeList;
}
