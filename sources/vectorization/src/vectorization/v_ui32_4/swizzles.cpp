#include "vectorization/v_ui32_4.h"

namespace vectorization {
  template <>
  v_ui32_4 swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(const v_ui32_4 &vector
  ) noexcept {
    return vector;
  }

  template <>
  v_ui32_4 swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y>(const v_ui32_4 &vector
  ) noexcept {
    return _mm_unpacklo_epi32(vector.components, vector.components);
  }

  template <>
  v_ui32_4 swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(const v_ui32_4 &vector
  ) noexcept {
    return _mm_unpackhi_epi32(vector.components, vector.components);
  }

  template <>
  v_ui32_4 swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(const v_ui32_4 &vector
  ) noexcept {
    return _mm_unpacklo_epi64(vector.components, vector.components);
  }

  template <>
  v_ui32_4 swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(const v_ui32_4 &vector
  ) noexcept {
    return _mm_unpackhi_epi64(vector.components, vector.components);
  }

  v_ui32_4 xxxx(const v_ui32_4 &vector) noexcept {
    return swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::X, VectorIndices::X>(vector);
  }

  v_ui32_4 yyyy(const v_ui32_4 &vector) noexcept {
    return swizzle<VectorIndices::Y, VectorIndices::Y, VectorIndices::Y, VectorIndices::Y>(vector);
  }

  v_ui32_4 zzzz(const v_ui32_4 &vector) noexcept {
    return swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::Z, VectorIndices::Z>(vector);
  }

  v_ui32_4 wwww(const v_ui32_4 &vector) noexcept {
    return swizzle<VectorIndices::W, VectorIndices::W, VectorIndices::W, VectorIndices::W>(vector);
  }

  v_ui32_4 xxyy(const v_ui32_4 &vector) noexcept {
    return swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y>(vector);
  }

  v_ui32_4 xyxy(const v_ui32_4 &vector) noexcept {
    return swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(vector);
  }

  v_ui32_4 yxwz(const v_ui32_4 &vector) noexcept {
    return swizzle<VectorIndices::Y, VectorIndices::X, VectorIndices::W, VectorIndices::Z>(vector);
  }

  v_ui32_4 zzww(const v_ui32_4 &vector) noexcept {
    return swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(vector);
  }

  v_ui32_4 zwzw(const v_ui32_4 &vector) noexcept {
    return swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(vector);
  }
}
