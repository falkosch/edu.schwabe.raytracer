#pragma once

#include "type.h"

namespace vectorization {
  template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W, bool SelectX, bool SelectY, bool SelectZ, bool SelectW>
  // generic swizzled blend
  v_i32_4 swizzledBlend(const v_i32_4 &a, const v_i32_4 &b) noexcept {
    return blend<SelectX, SelectY, SelectZ, SelectW>(swizzle<X, Y, Z, W>(a), swizzle<X, Y, Z, W>(b));
  }

  template <>
  v_i32_4
  swizzledBlend<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y, false, true, false, true>(
      const v_i32_4 &a, const v_i32_4 &b
  ) noexcept;

  template <>
  v_i32_4
  swizzledBlend<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y, true, false, true, false>(
      const v_i32_4 &a, const v_i32_4 &b
  ) noexcept;

  template <>
  v_i32_4
  swizzledBlend<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W, false, true, false, true>(
      const v_i32_4 &a, const v_i32_4 &b
  ) noexcept;

  template <>
  v_i32_4
  swizzledBlend<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W, true, false, true, false>(
      const v_i32_4 &a, const v_i32_4 &b
  ) noexcept;

  template <>
  v_i32_4
  swizzledBlend<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y, false, false, true, true>(
      const v_i32_4 &a, const v_i32_4 &b
  ) noexcept;

  template <>
  v_i32_4
  swizzledBlend<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y, true, true, false, false>(
      const v_i32_4 &a, const v_i32_4 &b
  ) noexcept;

  template <>
  v_i32_4
  swizzledBlend<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W, false, false, true, true>(
      const v_i32_4 &a, const v_i32_4 &b
  ) noexcept;

  template <>
  v_i32_4
  swizzledBlend<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W, true, true, false, false>(
      const v_i32_4 &a, const v_i32_4 &b
  ) noexcept;

  template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W>
  // Special case swizzled blend of two vectors, in which only the first
  // two components of this vector and the last two components of the
  // other vector are taken for blending after swizzling.
  v_i32_4 swizzledBlend2x2(const v_i32_4 &a, const v_i32_4 &b) noexcept {
    return swizzledBlend2x2<X, Y, Z, W, false, false, true, true>(a, b);
  }

  template <>
  v_i32_4 swizzledBlend2x2<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(
      const v_i32_4 &a, const v_i32_4 &b
  ) noexcept;

  template <>
  v_i32_4 swizzledBlend2x2<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(
      const v_i32_4 &a, const v_i32_4 &b
  ) noexcept;

  template <>
  v_i32_4 swizzledBlend2x2<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(
      const v_i32_4 &a, const v_i32_4 &b
  ) noexcept;

  v_i32_4 xy_xy(const v_i32_4 &a, const v_i32_4 &b) noexcept;
  v_i32_4 zw_zw(const v_i32_4 &a, const v_i32_4 &b) noexcept;

  template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W>
  v_i32_4 swizzledBlendMasked(const v_i32_4 &a, const v_i32_4 &b, const v_i32_4 &mask) noexcept {
    return blendMasked(swizzle<X, Y, Z, W>(a), swizzle<X, Y, Z, W>(b), mask);
  }
}
