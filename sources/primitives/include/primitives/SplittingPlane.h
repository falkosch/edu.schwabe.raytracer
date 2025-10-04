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

  AxisAlignedBoundingBox bounding(const SplittingPlane &plane) noexcept;

  // Checks for an intersection of a box with this plane.
  bool overlaps(const AxisAlignedBoundingBox &box, const SplittingPlane &plane) noexcept;
  bool overlaps(const RayCast &rayCast, const SplittingPlane &plane) noexcept;

  Float nearestIntersection(const RayCast &rayCast, const SplittingPlane &plane, Size2::ValueType originId) noexcept;
}
