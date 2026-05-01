#include "vectorization/functions/multiply.h"

namespace vectorization {
  PackedFloat4_128 multiply(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept {
    return _mm_mul_ps(a, b);
  }

  PackedFloat2_128 multiply(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept {
    return _mm_mul_pd(a, b);
  }

  PackedFloat8_256 multiply(const PackedFloat8_256 &a, const PackedFloat8_256 &b) noexcept {
    return _mm256_mul_ps(a, b);
  }

  PackedFloat4_256 multiply(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept {
    return _mm256_mul_pd(a, b);
  }

  PackedFloat4_128 multiplyX(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept {
    return _mm_mul_ss(a, b);
  }

  PackedFloat2_128 multiplyX(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept {
    return _mm_mul_sd(a, b);
  }
}
