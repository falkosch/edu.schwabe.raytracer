#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <bool SelectX, bool SelectY, bool SelectZ, bool SelectW>
  // generic blend of two vectors
  inline PackedFloat4_128 blend(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept {
    constexpr auto blendMask = (SelectX & 1) | ((SelectY & 1) << 1) | ((SelectZ & 1) << 2) | ((SelectW & 1) << 3);
    return _mm_blend_ps(a, b, blendMask);
  }

  template <>
  // passthrough a
  PackedFloat4_128 blend<false, false, false, false>(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept;

  template <>
  // passthrough b
  PackedFloat4_128 blend<true, true, true, true>(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept;

  template <>
  // shuffle x(a), y(a), z(b), w(b)
  PackedFloat4_128 blend<false, false, true, true>(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept;

  template <>
  // move x(a) into b
  PackedFloat4_128 blend<false, true, true, true>(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept;

  template <>
  // move x(b) into a
  PackedFloat4_128 blend<true, false, false, false>(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept;

  template <>
  // shuffle x(b), y(b), z(a), w(a)
  PackedFloat4_128 blend<true, true, false, false>(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept;

  PackedFloat4_128 x_yzw(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept;
}
