#pragma once

#include "type.h"

namespace vectorization {
  template <ASizeT X, ASizeT Y>
  v_i64_2 swizzle(const v_i64_2 &vector) noexcept;

  template <>
  v_i64_2 swizzle<VectorIndices::X, VectorIndices::X>(const v_i64_2 &vector) noexcept;

  template <>
  v_i64_2 swizzle<VectorIndices::X, VectorIndices::Y>(const v_i64_2 &vector) noexcept;

  template <>
  v_i64_2 swizzle<VectorIndices::Y, VectorIndices::X>(const v_i64_2 &vector) noexcept;

  template <>
  v_i64_2 swizzle<VectorIndices::Y, VectorIndices::Y>(const v_i64_2 &vector) noexcept;

  v_i64_2 xx(const v_i64_2 &vector) noexcept;
  v_i64_2 xy(const v_i64_2 &vector) noexcept;
  v_i64_2 yx(const v_i64_2 &vector) noexcept;
  v_i64_2 yy(const v_i64_2 &vector) noexcept;
}
