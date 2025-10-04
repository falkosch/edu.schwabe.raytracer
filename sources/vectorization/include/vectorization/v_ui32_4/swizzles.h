#pragma once

#include "type.h"

namespace vectorization {

  template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W>
  // generic swizzle
  v_ui32_4 swizzle(const v_ui32_4 &vector) noexcept {
    static_assert(X < v_ui32_4::SIZE, "Index is out of range");
    static_assert(Y < v_ui32_4::SIZE, "Index is out of range");
    static_assert(Z < v_ui32_4::SIZE, "Index is out of range");
    static_assert(W < v_ui32_4::SIZE, "Index is out of range");
    return _mm_shuffle_epi32(vector.components, _MM_SHUFFLE(W, Z, Y, X));
  }

  template <>
  // pass through swizzle
  v_ui32_4 swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(const v_ui32_4 &vector
  ) noexcept;

  template <>
  // special unpack low-values swizzle
  v_ui32_4 swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y>(const v_ui32_4 &vector
  ) noexcept;

  template <>
  // special unpack high-values swizzle
  v_ui32_4 swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(const v_ui32_4 &vector
  ) noexcept;

  template <>
  // special unpack XY-values swizzle
  v_ui32_4 swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(const v_ui32_4 &vector
  ) noexcept;

  template <>
  // special unpack ZW-values swizzle
  v_ui32_4 swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(const v_ui32_4 &vector
  ) noexcept;

  v_ui32_4 xxxx(const v_ui32_4 &vector) noexcept;
  v_ui32_4 yyyy(const v_ui32_4 &vector) noexcept;
  v_ui32_4 zzzz(const v_ui32_4 &vector) noexcept;
  v_ui32_4 wwww(const v_ui32_4 &vector) noexcept;
  v_ui32_4 xxyy(const v_ui32_4 &vector) noexcept;
  v_ui32_4 xyxy(const v_ui32_4 &vector) noexcept;
  v_ui32_4 yxwz(const v_ui32_4 &vector) noexcept;
  v_ui32_4 zzww(const v_ui32_4 &vector) noexcept;
  v_ui32_4 zwzw(const v_ui32_4 &vector) noexcept;
}
