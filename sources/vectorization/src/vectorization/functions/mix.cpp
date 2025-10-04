#include "vectorization/functions/mix.h"

namespace vectorization {
  const PackedFloat4_128 mix(const PackedFloat4_128 &x, const PackedFloat4_128 &y, const Float_32 factor) noexcept {
    return mix(x, y, _mm_set_ps1(factor));
  }

  const PackedFloat4_128
  mix(const PackedFloat4_128 &x, const PackedFloat4_128 &y, const PackedFloat4_128 &factor) noexcept {
    return _mm_add_ps(x, _mm_mul_ps(_mm_sub_ps(y, x), factor));
  }

  const PackedFloat2_128 mix(const PackedFloat2_128 &x, const PackedFloat2_128 &y, const Float_64 factor) noexcept {
    return mix(x, y, _mm_set1_pd(factor));
  }

  const PackedFloat2_128
  mix(const PackedFloat2_128 &x, const PackedFloat2_128 &y, const PackedFloat2_128 &factor) noexcept {
    return _mm_add_pd(x, _mm_mul_pd(_mm_sub_pd(y, x), factor));
  }

  const Float_32 mix(const Float_32 x, const Float_32 y, const Float_32 factor) noexcept {
    return x + (y - x) * factor;
  }

  const Float_64 mix(const Float_64 x, const Float_64 y, const Float_64 factor) noexcept {
    return x + (y - x) * factor;
  }
}