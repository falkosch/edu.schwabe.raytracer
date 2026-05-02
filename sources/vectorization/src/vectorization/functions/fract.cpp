#include "vectorization/functions/fract.h"

#include "vectorization/functions/floor.h"

namespace vectorization {
  Float_32 fract(const Float_32 value) noexcept {
    return value - floor(value);
  }

  Float_64 fract(const Float_64 value) noexcept {
    return value - floor(value);
  }

  PackedFloat4_128 fract(const PackedFloat4_128 &values) noexcept {
    return _mm_sub_ps(values, floor(values));
  }

  PackedFloat2_128 fract(const PackedFloat2_128 &values) noexcept {
    return _mm_sub_pd(values, floor(values));
  }

  PackedFloat8_256 fract(const PackedFloat8_256 &values) noexcept {
    return _mm256_sub_ps(values, floor(values));
  }

  PackedFloat4_256 fract(const PackedFloat4_256 &values) noexcept {
    return _mm256_sub_pd(values, floor(values));
  }
}
