#pragma once

#include "../FacetIntersection.h"

#include <primitives.h>

#include <vector>

namespace raytracer {
  using namespace vectorization;
  using namespace primitives;

  struct GeometryNode : public Intersectable<RayCast, FacetIntersection> {
    virtual ~GeometryNode();

    virtual const AxisAlignedBoundingBox includeInBounding(const AxisAlignedBoundingBox &aabb) const = 0;

    virtual const bool overlaps(const AxisAlignedBoundingBox &aabb) const = 0;

    virtual const Float getIndividualIntersectionCosts() const = 0;
  };

  typedef std::vector<GeometryNode *, AlignedAllocator<GeometryNode *>> PGeometryNodeList;
}
