#include "vectorization/functions/horizontal_subtract.h"

namespace vectorization {
  PackedFloat4_128 horizontalSubtract(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept {
    return _mm_hsub_ps(a, b);
  }

  PackedFloat2_128 horizontalSubtract(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept {
    return _mm_hsub_pd(a, b);
  }

  PackedFloat8_256 horizontalSubtract(const PackedFloat8_256 &a, const PackedFloat8_256 &b) noexcept {
    return _mm256_hsub_ps(a, b);
  }

  PackedFloat4_256 horizontalSubtract(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept {
    return _mm256_hsub_pd(a, b);
  }
}
