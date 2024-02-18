#pragma once

#include "AxisAlignedBoundingBox.h"

namespace primitives {
  using namespace vectorization;

  struct BoundingSphere {
    // contains the XYZ-coords of the sphere's center and the radius as the w-component
    Float4 centerRadius;

    BoundingSphere() noexcept;

    explicit BoundingSphere(const Float4 &center, Float radius) noexcept;
  };

  AxisAlignedBoundingBox bounding(const BoundingSphere &b) noexcept;

  bool overlaps(const RayCast &rayCast, const BoundingSphere &by) noexcept;

  Float nearestIntersection(const RayCast &r, const BoundingSphere &b, Size2::ValueType originId) noexcept;
}
