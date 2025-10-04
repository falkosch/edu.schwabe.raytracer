#pragma once

#include "RayCast.h"

namespace primitives {
  using namespace vectorization;

  struct AxisAlignedBoundingBox {
    Float4 minimum, maximum;

    AxisAlignedBoundingBox() noexcept;
    explicit AxisAlignedBoundingBox(const Float4 &a, const Float4 &b) noexcept;
  };

  Float4 extents(const AxisAlignedBoundingBox &box) noexcept;

  Float4 halfLengths(const AxisAlignedBoundingBox &box) noexcept;

  Float4 center(const AxisAlignedBoundingBox &box) noexcept;
  Float4 center(const AxisAlignedBoundingBox &box, const Float4 &halfLengths) noexcept;

  Float surfaceArea(const AxisAlignedBoundingBox &box) noexcept;
  /**
   * @param box the box
   * @param extents \c extents(box)
   * @return surface area for the rectangular cuboid of \p box, i.e. \c A=2(z*y+z*x+y*x)
   */
  Float surfaceArea(const AxisAlignedBoundingBox &box, const Float4 &extents) noexcept;

  AxisAlignedBoundingBox extendBy(const AxisAlignedBoundingBox &box, const Float4 &by) noexcept;
  AxisAlignedBoundingBox extendBy(const AxisAlignedBoundingBox &box, const Float4 &byMin, const Float4 &byMax) noexcept;
  AxisAlignedBoundingBox extendBy(const AxisAlignedBoundingBox &box, const AxisAlignedBoundingBox &by) noexcept;

  AxisAlignedBoundingBox clamp(const AxisAlignedBoundingBox &box, const AxisAlignedBoundingBox &by) noexcept;

  /**
   * Based on: Transforming Axis-Aligned Bounding Boxes by Jim Arvo (Graphics Gems, Academic Press, 1990)
   * https://github.com/erich666/GraphicsGems/blob/master/gems/TransBox.c
   */
  AxisAlignedBoundingBox transform(const AxisAlignedBoundingBox &box, const Float44 &by) noexcept;

  bool overlaps(const AxisAlignedBoundingBox &box, const Float4 &by) noexcept;
  /**
   * Tests whether two boxes overlap but does not compute intersection coefficients.
   * @return \c true if \p a and \p b intersect, otherwise \c false
   */
  bool overlaps(const AxisAlignedBoundingBox &a, const AxisAlignedBoundingBox &b) noexcept;
  bool overlaps(const RayCast &rayCast, const AxisAlignedBoundingBox &box) noexcept;
  Float4::VectorBoolType
  overlaps(const RayCast &rayCast, const AxisAlignedBoundingBox &a, const AxisAlignedBoundingBox &b) noexcept;

  Float
  nearestIntersection(const RayCast &rayCast, const AxisAlignedBoundingBox &box, Size2::ValueType originId) noexcept;
}
