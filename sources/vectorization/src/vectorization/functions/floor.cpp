#include "vectorization/functions/floor.h"

#include "vectorization/accessors.h"
#include "vectorization/constants.h"

namespace vectorization {
  const Float_32 floor(const Float_32 v) noexcept {
    return x(_mm_round_ss(Zero<PackedFloat4_128>(), _mm_set_ss(v), _MM_FROUND_TO_NEG_INF));
  }

  const Float_64 floor(const Float_64 v) noexcept {
    return x(_mm_round_sd(Zero<PackedFloat2_128>(), _mm_set_sd(v), _MM_FROUND_TO_NEG_INF));
  }

  const PackedFloat4_128 floor(const PackedFloat4_128 &v) noexcept {
    return _mm_round_ps(v, _MM_FROUND_TO_NEG_INF);
  }

  const PackedFloat2_128 floor(const PackedFloat2_128 &v) noexcept {
    return _mm_round_pd(v, _MM_FROUND_TO_NEG_INF);
  }
}