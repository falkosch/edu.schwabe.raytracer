#pragma once

#include "vectorization/swizzledBlends/swizzledBlend_256d.h"

#include "vectorization/architecture.h"

namespace vectorization {
  // Special case swizzled blend of two vectors, in which only the first
  // two components of this vector and the last two components of the
  // other vector are taken for blending after swizzling.
  template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W>
  inline PackedFloat4_256 swizzledBlend2x2(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept {
    static_assert(X < VectorSizes::W, "Index is out of range");
    static_assert(Y < VectorSizes::W, "Index is out of range");
    static_assert(Z < VectorSizes::W, "Index is out of range");
    static_assert(W < VectorSizes::W, "Index is out of range");
    return swizzledBlend<X, Y, Z, W, false, false, true, true>(a, b);
  }

  PackedFloat4_256 xx_xx(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept;
  PackedFloat4_256 xx_yy(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept;
  PackedFloat4_256 xy_xy(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept;
  PackedFloat4_256 xy_zw(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept;
  PackedFloat4_256 xz_xz(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept;
  PackedFloat4_256 yx_xy(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept;
  PackedFloat4_256 yy_yy(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept;
  PackedFloat4_256 yw_yw(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept;
  PackedFloat4_256 zz_yy(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept;
  PackedFloat4_256 zz_zz(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept;
  PackedFloat4_256 zw_zw(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept;
  PackedFloat4_256 ww_ww(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept;
}
