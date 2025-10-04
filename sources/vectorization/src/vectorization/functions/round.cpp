#include "vectorization/functions/round.h"

#include "vectorization/accessors.h"
#include "vectorization/constants.h"

namespace vectorization {
  Float_32 round(const Float_32 value) noexcept {
    return x(_mm_round_ss(Zero<PackedFloat4_128>(), _mm_set_ss(value), _MM_FROUND_TO_NEAREST_INT));
  }

  Float_64 round(const Float_64 value) noexcept {
    return x(_mm_round_sd(Zero<PackedFloat2_128>(), _mm_set_sd(value), _MM_FROUND_TO_NEAREST_INT));
  }

  PackedFloat4_128 round(const PackedFloat4_128 &values) noexcept {
    return _mm_round_ps(values, _MM_FROUND_TO_NEAREST_INT);
  }

  PackedFloat2_128 round(const PackedFloat2_128 &values) noexcept {
    return _mm_round_pd(values, _MM_FROUND_TO_NEAREST_INT);
  }
}
