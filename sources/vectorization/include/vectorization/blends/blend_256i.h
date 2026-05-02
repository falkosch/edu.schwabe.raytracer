#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <bool SelectX1, bool SelectX2, bool SelectX3, bool SelectX4,
            bool SelectX5, bool SelectX6, bool SelectX7, bool SelectX8>
  PackedInts_256 blend(const PackedInts_256 &a, const PackedInts_256 &b) noexcept {
    constexpr auto blendMask = (SelectX1 & 1) | ((SelectX2 & 1) << 1) | ((SelectX3 & 1) << 2) | ((SelectX4 & 1) << 3)
                               | ((SelectX5 & 1) << 4) | ((SelectX6 & 1) << 5) | ((SelectX7 & 1) << 6)
                               | ((SelectX8 & 1) << 7);
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_blend_epi32(a, b, blendMask);
#else
    return _mm256_castps_si256(
        _mm256_blend_ps(_mm256_castsi256_ps(a), _mm256_castsi256_ps(b), blendMask)
    );
#endif
  }

  template <>
  PackedInts_256 blend<false, false, false, false, false, false, false, false>(
      const PackedInts_256 &a, const PackedInts_256 &b
  ) noexcept;

  template <>
  PackedInts_256
  blend<true, true, true, true, true, true, true, true>(const PackedInts_256 &a, const PackedInts_256 &b) noexcept;
}
