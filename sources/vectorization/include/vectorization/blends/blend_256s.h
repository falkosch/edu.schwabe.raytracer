#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  // generic blend of two vectors
  template <bool SelectX, bool SelectY, bool SelectZ, bool SelectW>
  inline PackedFloat8_256 blend(const PackedFloat8_256 &a, const PackedFloat8_256 &b) noexcept {
    constexpr auto blendMask = 0 | ((SelectX & 1) << 0) | ((SelectY & 1) << 1) | ((SelectZ & 1) << 2)
                               | ((SelectW & 1) << 3) | ((SelectX & 1) << 4) | ((SelectY & 1) << 5)
                               | ((SelectZ & 1) << 6) | ((SelectW & 1) << 7);
    return _mm256_blend_ps(a, b, blendMask);
  }

  // generic blend of two vectors
  template <
      bool SelectX1, bool SelectX2, bool SelectX3, bool SelectX4, bool SelectX5, bool SelectX6, bool SelectX7,
      bool SelectX8>
  inline PackedFloat8_256 blend(const PackedFloat8_256 &a, const PackedFloat8_256 &b) noexcept {
    constexpr auto blendMask = (SelectX1 & 1) | ((SelectX2 & 1) << 1) | ((SelectX3 & 1) << 2) | ((SelectX4 & 1) << 3)
                               | ((SelectX5 & 1) << 4) | ((SelectX6 & 1) << 5) | ((SelectX7 & 1) << 6)
                               | ((SelectX8 & 1) << 7);
    return _mm256_blend_ps(a, b, blendMask);
  }

  // passthrough a
  template <>
  PackedFloat8_256 blend<false, false, false, false, false, false, false, false>(
      const PackedFloat8_256 &a, const PackedFloat8_256 &b
  ) noexcept;

  // passthrough b
  template <>
  PackedFloat8_256
  blend<true, true, true, true, true, true, true, true>(const PackedFloat8_256 &a, const PackedFloat8_256 &b) noexcept;
}
