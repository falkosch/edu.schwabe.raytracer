#include "vectorization/swizzledBlends/swizzledBlend1x1_128d.h"

namespace vectorization {
  // x(a), x(b)
  template <>
  PackedFloat2_128
  swizzledBlend1x1<VectorIndices::X, VectorIndices::X>(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept {
    return _mm_unpacklo_pd(a, b);
  }

  // y(a), y(b)
  template <>
  PackedFloat2_128
  swizzledBlend1x1<VectorIndices::Y, VectorIndices::Y>(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept {
    return _mm_unpackhi_pd(a, b);
  }

  PackedFloat2_128 x_x(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept {
    return swizzledBlend1x1<VectorIndices::X, VectorIndices::X>(a, b);
  }

  PackedFloat2_128 x_y(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept {
    return swizzledBlend1x1<VectorIndices::X, VectorIndices::Y>(a, b);
  }

  PackedFloat2_128 y_x(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept {
    return swizzledBlend1x1<VectorIndices::Y, VectorIndices::X>(a, b);
  }

  PackedFloat2_128 y_y(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept {
    return swizzledBlend1x1<VectorIndices::Y, VectorIndices::Y>(a, b);
  }
}
