#include "vectorization/functions/floor.h"

#include "vectorization/accessors.h"
#include "vectorization/constants.h"

namespace vectorization {
  Float_32 floor(const Float_32 value) noexcept {
    return x(_mm_round_ss(Zero<PackedFloat4_128>(), _mm_set_ss(value), _MM_FROUND_TO_NEG_INF));
  }

  Float_64 floor(const Float_64 value) noexcept {
    return x(_mm_round_sd(Zero<PackedFloat2_128>(), _mm_set_sd(value), _MM_FROUND_TO_NEG_INF));
  }

  PackedFloat4_128 floor(const PackedFloat4_128 &values) noexcept {
    return _mm_round_ps(values, _MM_FROUND_TO_NEG_INF);
  }

  PackedFloat2_128 floor(const PackedFloat2_128 &values) noexcept {
    return _mm_round_pd(values, _MM_FROUND_TO_NEG_INF);
  }
}
