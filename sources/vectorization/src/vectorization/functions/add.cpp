#include "vectorization/functions/add.h"

namespace vectorization {
  PackedFloat4_128 add(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept {
    return _mm_add_ps(a, b);
  }

  PackedFloat2_128 add(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept {
    return _mm_add_pd(a, b);
  }

  PackedFloat8_256 add(const PackedFloat8_256 &a, const PackedFloat8_256 &b) noexcept {
    return _mm256_add_ps(a, b);
  }

  PackedFloat4_256 add(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept {
    return _mm256_add_pd(a, b);
  }

  PackedFloat4_128 addX(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept {
    return _mm_add_ss(a, b);
  }

  PackedFloat2_128 addX(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept {
    return _mm_add_sd(a, b);
  }
}
