#pragma once

#include "type.h"
#include "vectorization/swizzles.h"

namespace vectorization {
  template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W>
  v_f32_8 swizzle(const v_f32_8 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return swizzle<X, Y, Z, W>(v.components);
#else
    return {{swizzle<X, Y, Z, W>(v.components.lo), swizzle<X, Y, Z, W>(v.components.hi)}};
#endif
  }

  template <>
  v_f32_8 swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(const v_f32_8 &v) noexcept;

  v_f32_8 xxxx(const v_f32_8 &v) noexcept;
  v_f32_8 yyyy(const v_f32_8 &v) noexcept;
  v_f32_8 zzzz(const v_f32_8 &v) noexcept;
  v_f32_8 wwww(const v_f32_8 &v) noexcept;
  v_f32_8 xxyy(const v_f32_8 &v) noexcept;
  v_f32_8 xyxy(const v_f32_8 &v) noexcept;
  v_f32_8 yyww(const v_f32_8 &v) noexcept;
  v_f32_8 zzww(const v_f32_8 &v) noexcept;
  v_f32_8 zwzw(const v_f32_8 &v) noexcept;
}
