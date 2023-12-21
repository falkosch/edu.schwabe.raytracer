#include "vectorization/functions/sqrt.h"

#include "vectorization/accessors.h"
#include "vectorization/constants.h"

namespace vectorization {
  const Float_32 sqrt(const Float_32 v) noexcept {
    return x(_mm_sqrt_ss(_mm_set_ss(v)));
  }

  const Float_64 sqrt(const Float_64 v) noexcept {
    return x(_mm_sqrt_sd(_mm_set_sd(v), Zero<PackedFloat2_128>()));
  }

  const PackedFloat4_128 sqrt(const PackedFloat4_128 &v) noexcept {
    return _mm_sqrt_ps(v);
  }

  const PackedFloat2_128 sqrt(const PackedFloat2_128 &v) noexcept {
    return _mm_sqrt_pd(v);
  }
}