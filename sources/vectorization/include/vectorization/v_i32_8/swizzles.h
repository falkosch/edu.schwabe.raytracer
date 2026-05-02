#pragma once

#include "type.h"

namespace vectorization {

  template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W>
  v_i32_8 swizzle(const v_i32_8 &v) noexcept;

  template <>
  v_i32_8 swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(const v_i32_8 &v) noexcept;

  v_i32_8 xxxx(const v_i32_8 &v) noexcept;
  v_i32_8 yyyy(const v_i32_8 &v) noexcept;
  v_i32_8 zzzz(const v_i32_8 &v) noexcept;
  v_i32_8 wwww(const v_i32_8 &v) noexcept;
  v_i32_8 xxyy(const v_i32_8 &v) noexcept;
  v_i32_8 xyxy(const v_i32_8 &v) noexcept;
  v_i32_8 yyww(const v_i32_8 &v) noexcept;
  v_i32_8 zzww(const v_i32_8 &v) noexcept;
  v_i32_8 zwzw(const v_i32_8 &v) noexcept;
}
