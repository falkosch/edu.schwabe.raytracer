#pragma once

#include "vectorization/architecture.h"
#include "vectorization/blends.h"
#include "vectorization/swizzles.h"

namespace vectorization {
  // Special case swizzled blend of two vectors, in which only the first
  // component of this vector and the last component of the other vector are
  // taken for blending after swizzling.
  template <ASizeT X, ASizeT Y>
  inline const PackedFloat2_128 swizzledBlend1x1(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept {
    static_assert(X < VectorSizes::W, "Index is out of range");
    static_assert(Y < VectorSizes::W, "Index is out of range");
    const auto shuffleMask = ((X & 1) << 0) | ((Y & 1) << 1);
    return _mm_shuffle_pd(a, b, shuffleMask);
  }

  // x(a), x(b)
  template <>
  const PackedFloat2_128
  swizzledBlend1x1<VectorIndices::X, VectorIndices::X>(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept;

  // y(a), y(b)
  template <>
  const PackedFloat2_128
  swizzledBlend1x1<VectorIndices::Y, VectorIndices::Y>(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept;

  const PackedFloat2_128 x_x(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept;

  const PackedFloat2_128 x_y(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept;

  const PackedFloat2_128 y_x(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept;

  const PackedFloat2_128 y_y(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept;
}
