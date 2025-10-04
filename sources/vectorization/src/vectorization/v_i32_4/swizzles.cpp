#include "vectorization/v_i32_4.h"

namespace vectorization {
  template <>
  v_i32_4 swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(const v_i32_4 &v) noexcept {
    return v;
  }

  template <>
  v_i32_4 swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y>(const v_i32_4 &v) noexcept {
    return _mm_unpacklo_epi32(v.components, v.components);
  }

  template <>
  v_i32_4 swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(const v_i32_4 &v) noexcept {
    return _mm_unpackhi_epi32(v.components, v.components);
  }

  template <>
  v_i32_4 swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(const v_i32_4 &v) noexcept {
    return _mm_unpacklo_epi64(v.components, v.components);
  }

  template <>
  v_i32_4 swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(const v_i32_4 &v) noexcept {
    return _mm_unpackhi_epi64(v.components, v.components);
  }

  v_i32_4 xxxx(const v_i32_4 &v) noexcept {
    return swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::X, VectorIndices::X>(v);
  }

  v_i32_4 yyyy(const v_i32_4 &v) noexcept {
    return swizzle<VectorIndices::Y, VectorIndices::Y, VectorIndices::Y, VectorIndices::Y>(v);
  }

  v_i32_4 zzzz(const v_i32_4 &v) noexcept {
    return swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::Z, VectorIndices::Z>(v);
  }

  v_i32_4 wwww(const v_i32_4 &v) noexcept {
    return swizzle<VectorIndices::W, VectorIndices::W, VectorIndices::W, VectorIndices::W>(v);
  }

  v_i32_4 xxyy(const v_i32_4 &v) noexcept {
    return swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y>(v);
  }

  v_i32_4 xyxy(const v_i32_4 &v) noexcept {
    return swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(v);
  }

  v_i32_4 yxwz(const v_i32_4 &v) noexcept {
    return swizzle<VectorIndices::Y, VectorIndices::X, VectorIndices::W, VectorIndices::Z>(v);
  }

  v_i32_4 zzww(const v_i32_4 &v) noexcept {
    return swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(v);
  }

  v_i32_4 zwzw(const v_i32_4 &v) noexcept {
    return swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(v);
  }
}
