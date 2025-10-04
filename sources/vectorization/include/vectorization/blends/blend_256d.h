#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <bool SelectX, bool SelectY, bool SelectZ, bool SelectW>
  // generic blend of two vectors
  PackedFloat4_256 blend(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept {
    constexpr auto blendMask = (SelectX & 1) | ((SelectY & 1) << 1) | ((SelectZ & 1) << 2) | ((SelectW & 1) << 3);
    return _mm256_blend_pd(a, b, blendMask);
  }

  template <>
  // pass through blend a
  PackedFloat4_256 blend<false, false, false, false>(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept;

  template <>
  // pass through blend b
  PackedFloat4_256 blend<true, true, true, true>(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept;
}
