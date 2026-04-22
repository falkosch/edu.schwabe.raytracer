#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  inline PackedFloat4_128 broadcast(const Float_32 scalar) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm_broadcastss_ps(_mm_set_ss(scalar));
#else
    return _mm_set_ps1(scalar);
#endif
  }

  inline PackedFloat2_128 broadcast(const Float_64 scalar) noexcept {
    return _mm_set1_pd(scalar);
  }

  inline PackedInts_128 broadcast(const Int_32 scalar) noexcept {
    return _mm_set1_epi32(scalar);
  }
}
