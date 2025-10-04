#include "vectorization/swizzles/swizzle_128d.h"

namespace vectorization {
  template <>
  PackedFloat2_128 swizzle<VectorIndices::X, VectorIndices::Y>(const PackedFloat2_128 &values) noexcept {
    return values;
  }

  template <>
  PackedFloat2_128 swizzle<VectorIndices::X, VectorIndices::X>(const PackedFloat2_128 &values) noexcept {
    return _mm_movedup_pd(values);
  }

  template <>
  PackedFloat2_128 swizzle<VectorIndices::Y, VectorIndices::Y>(const PackedFloat2_128 &values) noexcept {
    return _mm_unpackhi_pd(values, values);
  }

  PackedFloat2_128 xx(const PackedFloat2_128 &values) noexcept {
    return swizzle<VectorIndices::X, VectorIndices::X>(values);
  }

  PackedFloat2_128 yx(const PackedFloat2_128 &values) noexcept {
    return swizzle<VectorIndices::Y, VectorIndices::X>(values);
  }

  PackedFloat2_128 yy(const PackedFloat2_128 &values) noexcept {
    return swizzle<VectorIndices::Y, VectorIndices::Y>(values);
  }
}
