#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <ASizeT X, ASizeT Y>
  // generic swizzle
  inline PackedFloat2_128 swizzle(const PackedFloat2_128 &values) noexcept {
    static_assert(X < VectorSizes::Y, "Index X is out of range");
    static_assert(Y < VectorSizes::Y, "Index Y is out of range");
    constexpr auto swizzleMask = X + (Y << 1);
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return _mm_permute_pd(values, swizzleMask);
#else
    return _mm_shuffle_pd(v, v, swizzleMask);
#endif
  }

  template <>
  // pass through swizzle
  PackedFloat2_128 swizzle<VectorIndices::X, VectorIndices::Y>(const PackedFloat2_128 &values) noexcept;

  template <>
  // duplicate low swizzle
  PackedFloat2_128 swizzle<VectorIndices::X, VectorIndices::X>(const PackedFloat2_128 &values) noexcept;

  template <>
  // unpack high swizzle
  PackedFloat2_128 swizzle<VectorIndices::Y, VectorIndices::Y>(const PackedFloat2_128 &values) noexcept;

  PackedFloat2_128 xx(const PackedFloat2_128 &values) noexcept;
  PackedFloat2_128 yx(const PackedFloat2_128 &values) noexcept;
  PackedFloat2_128 yy(const PackedFloat2_128 &values) noexcept;
}
