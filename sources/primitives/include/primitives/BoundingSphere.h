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

  AxisAlignedBoundingBox bounding(const BoundingSphere &sphere) noexcept;

  bool overlaps(const RayCast &rayCast, const BoundingSphere &sphere) noexcept;

  Float nearestIntersection(const RayCast &rayCast, const BoundingSphere &sphere, Size2::ValueType originId) noexcept;
}
