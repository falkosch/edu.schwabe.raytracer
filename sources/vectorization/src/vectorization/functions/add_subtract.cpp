#include "vectorization/functions/add_subtract.h"

namespace vectorization {
  PackedFloat4_128 addSubtract(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept {
    return _mm_addsub_ps(a, b);
  }

  PackedFloat2_128 addSubtract(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept {
    return _mm_addsub_pd(a, b);
  }

  PackedFloat8_256 addSubtract(const PackedFloat8_256 &a, const PackedFloat8_256 &b) noexcept {
    return _mm256_addsub_ps(a, b);
  }

  PackedFloat4_256 addSubtract(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept {
    return _mm256_addsub_pd(a, b);
  }
}
