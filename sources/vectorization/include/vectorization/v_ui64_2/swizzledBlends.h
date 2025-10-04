#pragma once

#include "type.h"

#include "vectorization/swizzledBlends.h"

namespace vectorization {
  // generic swizzled blend
  template <ASizeT X, ASizeT Y, bool SelectX, bool SelectY>
  v_ui64_2 swizzledBlend(const v_ui64_2 &a, const v_ui64_2 &b) noexcept {
    return blend<SelectX, SelectY>(swizzle<X, Y>(a), swizzle<X, Y>(b));
  }

  template <>
  v_ui64_2
  swizzledBlend<VectorIndices::X, VectorIndices::X, false, true>(const v_ui64_2 &a, const v_ui64_2 &b) noexcept;

  template <>
  v_ui64_2
  swizzledBlend<VectorIndices::X, VectorIndices::X, true, false>(const v_ui64_2 &a, const v_ui64_2 &b) noexcept;

  template <>
  v_ui64_2
  swizzledBlend<VectorIndices::Y, VectorIndices::Y, false, true>(const v_ui64_2 &a, const v_ui64_2 &b) noexcept;

  template <>
  v_ui64_2
  swizzledBlend<VectorIndices::Y, VectorIndices::Y, true, false>(const v_ui64_2 &a, const v_ui64_2 &b) noexcept;

  // Special case swizzled blend of two vectors, in which only the first
  // two components of this vector and the last two components of the
  // other vector are taken for blending after swizzling.
  template <ASizeT X, ASizeT Y>
  v_ui64_2 swizzledBlend1x1(const v_ui64_2 &a, const v_ui64_2 &b) noexcept;

  template <>
  v_ui64_2 swizzledBlend1x1<VectorIndices::X, VectorIndices::X>(const v_ui64_2 &a, const v_ui64_2 &b) noexcept;

  template <>
  v_ui64_2 swizzledBlend1x1<VectorIndices::X, VectorIndices::Y>(const v_ui64_2 &a, const v_ui64_2 &b) noexcept;

  template <>
  v_ui64_2 swizzledBlend1x1<VectorIndices::Y, VectorIndices::X>(const v_ui64_2 &a, const v_ui64_2 &b) noexcept;

  template <>
  v_ui64_2 swizzledBlend1x1<VectorIndices::Y, VectorIndices::Y>(const v_ui64_2 &a, const v_ui64_2 &b) noexcept;

  v_ui64_2 x_x(const v_ui64_2 &a, const v_ui64_2 &b) noexcept;
  v_ui64_2 x_y(const v_ui64_2 &a, const v_ui64_2 &b) noexcept;
  v_ui64_2 y_x(const v_ui64_2 &a, const v_ui64_2 &b) noexcept;
  v_ui64_2 y_y(const v_ui64_2 &a, const v_ui64_2 &b) noexcept;

  // generic swizzle with variadic mask for blend
  template <ASizeT X, ASizeT Y>
  v_ui64_2 swizzledBlendMasked(const v_ui64_2 &a, const v_ui64_2 &b, const v_ui64_2 &mask) noexcept {
    return blendMasked(swizzle<X, Y>(a), swizzle<X, Y>(b), mask);
  }

  // pass through
  template <>
  v_ui64_2 swizzledBlendMasked<VectorIndices::X, VectorIndices::Y>(
      const v_ui64_2 &a, const v_ui64_2 &b, const v_ui64_2 &mask
  ) noexcept;
}
