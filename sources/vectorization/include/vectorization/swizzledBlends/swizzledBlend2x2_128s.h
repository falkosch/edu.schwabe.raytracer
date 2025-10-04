#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  // Special case swizzled blend of two vectors, in which only the first
  // two components of this vector and the last two components of the
  // other vector are taken for blending after swizzling.
  template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W>
  PackedFloat4_128 swizzledBlend2x2(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept {
    static_assert(X < VectorSizes::W, "Index is out of range");
    static_assert(Y < VectorSizes::W, "Index is out of range");
    static_assert(Z < VectorSizes::W, "Index is out of range");
    static_assert(W < VectorSizes::W, "Index is out of range");
    return _mm_shuffle_ps(a, b, _MM_SHUFFLE(W, Z, Y, X));
  }

  template <>
  PackedFloat4_128 swizzledBlend2x2<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(
      const PackedFloat4_128 &a, const PackedFloat4_128 &b
  ) noexcept;

  template <>
  PackedFloat4_128 swizzledBlend2x2<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(
      const PackedFloat4_128 &a, const PackedFloat4_128 &b
  ) noexcept;

  PackedFloat4_128 xx_xx(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept;
  PackedFloat4_128 xx_yy(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept;
  PackedFloat4_128 xy_xy(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept;
  PackedFloat4_128 xy_zw(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept;
  PackedFloat4_128 xz_xz(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept;
  PackedFloat4_128 yx_xy(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept;
  PackedFloat4_128 yy_yy(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept;
  PackedFloat4_128 yw_yw(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept;
  PackedFloat4_128 zz_yy(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept;
  PackedFloat4_128 zz_zz(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept;
  PackedFloat4_128 zw_zw(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept;
  PackedFloat4_128 ww_ww(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept;
}
