#pragma once

#include "../partitioning/GeometryNode.h"

#include <primitives.h>

namespace raytracer {
  using namespace primitives;

  class Form : public GeometryNode {
  public:
    ~Form() override;

    virtual bool isInfinite() const;

    virtual AxisAlignedBoundingBox getBounding() const;

    AxisAlignedBoundingBox includeInBounding(const AxisAlignedBoundingBox &aabb) const override;

    bool overlaps(const AxisAlignedBoundingBox &aabb) const override;
  };
}
