#include "vectorization/v_i32_4.h"

namespace vectorization {
  template <>
  v_i32_4
  swizzledBlend<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y, false, true, false, true>(
      const v_i32_4 &a, const v_i32_4 &b
  ) noexcept {
    return _mm_unpacklo_epi32(a.components, b.components);
  }

  template <>
  v_i32_4
  swizzledBlend<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y, true, false, true, false>(
      const v_i32_4 &a, const v_i32_4 &b
  ) noexcept {
    // args a and b intentionally swapped here
    return _mm_unpacklo_epi32(b.components, a.components);
  }

  template <>
  v_i32_4
  swizzledBlend<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W, false, true, false, true>(
      const v_i32_4 &a, const v_i32_4 &b
  ) noexcept {
    return _mm_unpackhi_epi32(a.components, b.components);
  }

  template <>
  v_i32_4
  swizzledBlend<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W, true, false, true, false>(
      const v_i32_4 &a, const v_i32_4 &b
  ) noexcept {
    return _mm_unpackhi_epi32(b.components, a.components);
  }

  template <>
  v_i32_4
  swizzledBlend<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y, false, false, true, true>(
      const v_i32_4 &a, const v_i32_4 &b
  ) noexcept {
    return _mm_unpacklo_epi64(a.components, b.components);
  }

  template <>
  v_i32_4
  swizzledBlend<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y, true, true, false, false>(
      const v_i32_4 &a, const v_i32_4 &b
  ) noexcept {
    return _mm_unpacklo_epi64(b.components, a.components);
  }

  template <>
  v_i32_4
  swizzledBlend<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W, false, false, true, true>(
      const v_i32_4 &a, const v_i32_4 &b
  ) noexcept {
    return _mm_unpackhi_epi64(a.components, b.components);
  }

  template <>
  v_i32_4
  swizzledBlend<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W, true, true, false, false>(
      const v_i32_4 &a, const v_i32_4 &b
  ) noexcept {
    return _mm_unpackhi_epi64(b.components, a.components);
  }

  template <>
  v_i32_4 swizzledBlend2x2<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(
      const v_i32_4 &a, const v_i32_4 &b
  ) noexcept {
    return blend<false, false, true, true>(a, b);
  }

  template <>
  v_i32_4 swizzledBlend2x2<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(
      const v_i32_4 &a, const v_i32_4 &b
  ) noexcept {
    return _mm_unpacklo_epi64(a.components, b.components);
  }

  template <>
  v_i32_4 swizzledBlend2x2<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(
      const v_i32_4 &a, const v_i32_4 &b
  ) noexcept {
    return _mm_unpackhi_epi64(a.components, b.components);
  }

  v_i32_4 xy_xy(const v_i32_4 &a, const v_i32_4 &b) noexcept {
    return swizzledBlend2x2<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(a, b);
  }

  v_i32_4 zw_zw(const v_i32_4 &a, const v_i32_4 &b) noexcept {
    return swizzledBlend2x2<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(a, b);
  }
}
