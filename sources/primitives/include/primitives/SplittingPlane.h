#pragma once

#include "AxisAlignedBoundingBox.h"

namespace primitives {
  using namespace vectorization;

  struct SplittingPlane {
    Float4 normalDistance;

    SplittingPlane() noexcept;

    explicit SplittingPlane(const Float4 &normalDistance) noexcept;

    explicit SplittingPlane(const Float4 &origin, const Float4 &normal) noexcept;
  };

  AxisAlignedBoundingBox bounding(const SplittingPlane &p) noexcept;

  bool overlaps(const AxisAlignedBoundingBox &a, const SplittingPlane &p) noexcept;

  // Checks for intersection of this box with a plane.
  bool overlaps(const RayCast &rayCast, const SplittingPlane &by) noexcept;

  Float nearestIntersection(const RayCast &rayCast, const SplittingPlane &p, Size2::ValueType originId) noexcept;
}
