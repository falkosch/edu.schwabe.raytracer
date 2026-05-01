#include "vectorization/functions/horizontal_add.h"

namespace vectorization {
  PackedFloat4_128 horizontalAdd(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept {
    return _mm_hadd_ps(a, b);
  }

  PackedFloat2_128 horizontalAdd(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept {
    return _mm_hadd_pd(a, b);
  }

  PackedFloat8_256 horizontalAdd(const PackedFloat8_256 &a, const PackedFloat8_256 &b) noexcept {
    return _mm256_hadd_ps(a, b);
  }

  PackedFloat4_256 horizontalAdd(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept {
    return _mm256_hadd_pd(a, b);
  }
}
