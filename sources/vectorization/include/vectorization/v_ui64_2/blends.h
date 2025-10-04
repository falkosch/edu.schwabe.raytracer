#pragma once

#include "type.h"

#include "vectorization/blends.h"

namespace vectorization {
  // generic blend of two vectors
  template <bool SelectX, bool SelectY>
  v_ui64_2 blend(const v_ui64_2 &a, const v_ui64_2 &b) noexcept;

  template <>
  v_ui64_2 blend<false, false>(const v_ui64_2 &a, const v_ui64_2 &b) noexcept;

  template <>
  v_ui64_2 blend<false, true>(const v_ui64_2 &a, const v_ui64_2 &b) noexcept;

  template <>
  v_ui64_2 blend<true, false>(const v_ui64_2 &a, const v_ui64_2 &b) noexcept;

  template <>
  v_ui64_2 blend<true, true>(const v_ui64_2 &a, const v_ui64_2 &b) noexcept;

  // variadic blend
  v_ui64_2 blendMasked(const v_ui64_2 &a, const v_ui64_2 &b, const v_ui64_2 &mask) noexcept;
}
