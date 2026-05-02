#include "vectorization/functions/sqrt.h"

#include "vectorization/accessors/component_128d.h"
#include "vectorization/accessors/component_128s.h"
#include "vectorization/constants/values.h"

namespace vectorization {
  Float_32 sqrt(const Float_32 value) noexcept {
    return x(_mm_sqrt_ss(_mm_set_ss(value)));
  }

  Float_64 sqrt(const Float_64 value) noexcept {
    return x(_mm_sqrt_sd(Zero<PackedFloat2_128>(), _mm_set_sd(value)));
  }

  PackedFloat4_128 sqrt(const PackedFloat4_128 &values) noexcept {
    return _mm_sqrt_ps(values);
  }

  PackedFloat2_128 sqrt(const PackedFloat2_128 &values) noexcept {
    return _mm_sqrt_pd(values);
  }

  PackedFloat8_256 sqrt(const PackedFloat8_256 &values) noexcept {
    return _mm256_sqrt_ps(values);
  }

  PackedFloat4_256 sqrt(const PackedFloat4_256 &values) noexcept {
    return _mm256_sqrt_pd(values);
  }
}
