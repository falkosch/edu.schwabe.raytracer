#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <bool SelectX, bool SelectY>
  // generic blend of two vectors
  PackedFloat2_128 blend(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept;

  template <>
  // passthrough a
  PackedFloat2_128 blend<false, false>(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept;

  template <>
  // pass through b
  PackedFloat2_128 blend<true, true>(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept;

  template <>
  // x(a), y(b)
  PackedFloat2_128 blend<false, true>(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept;

  template <>
  // x(b), y(a)
  PackedFloat2_128 blend<true, false>(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept;
}
