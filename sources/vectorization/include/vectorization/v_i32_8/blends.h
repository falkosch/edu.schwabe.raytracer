#pragma once

#include "type.h"

namespace vectorization {

  template <bool SelectX1, bool SelectX2, bool SelectX3, bool SelectX4,
            bool SelectX5, bool SelectX6, bool SelectX7, bool SelectX8>
  v_i32_8 blend(const v_i32_8 &a, const v_i32_8 &b) noexcept;

  template <>
  v_i32_8 blend<false, false, false, false, false, false, false, false>(const v_i32_8 &a, const v_i32_8 &) noexcept;

  template <>
  v_i32_8 blend<true, true, true, true, true, true, true, true>(const v_i32_8 &, const v_i32_8 &b) noexcept;

  v_i32_8 blendMasked(const v_i32_8 &a, const v_i32_8 &b, const v_i32_8 &mask) noexcept;
}
