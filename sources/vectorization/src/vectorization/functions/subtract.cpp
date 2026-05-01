#include "vectorization/functions/subtract.h"

namespace vectorization {
  PackedFloat4_128 subtract(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept {
    return _mm_sub_ps(a, b);
  }

  PackedFloat2_128 subtract(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept {
    return _mm_sub_pd(a, b);
  }

  PackedFloat8_256 subtract(const PackedFloat8_256 &a, const PackedFloat8_256 &b) noexcept {
    return _mm256_sub_ps(a, b);
  }

  PackedFloat4_256 subtract(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept {
    return _mm256_sub_pd(a, b);
  }

  PackedFloat4_128 subtractX(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept {
    return _mm_sub_ss(a, b);
  }

  PackedFloat2_128 subtractX(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept {
    return _mm_sub_sd(a, b);
  }
}
