#include "vectorization/functions/sqrt.h"

#include "vectorization/accessors.h"
#include "vectorization/constants.h"

namespace vectorization {
  Float_32 sqrt(const Float_32 value) noexcept {
    return x(_mm_sqrt_ss(_mm_set_ss(value)));
  }

  Float_64 sqrt(const Float_64 value) noexcept {
    return x(_mm_sqrt_sd(_mm_set_sd(value), Zero<PackedFloat2_128>()));
  }

  PackedFloat4_128 sqrt(const PackedFloat4_128 &values) noexcept {
    return _mm_sqrt_ps(values);
  }

  PackedFloat2_128 sqrt(const PackedFloat2_128 &values) noexcept {
    return _mm_sqrt_pd(values);
  }
}
