#include "vectorization/selects/selects_128d.h"

#include "vectorization/blends/blend_masked_128d.h"
#include "vectorization/constants/values/zero.h"

namespace vectorization {
  PackedFloat2_128 select(const bool mask, const PackedFloat2_128 &onTrue, const PackedFloat2_128 &onFalse) noexcept {
    return mask ? onTrue : onFalse;
  }

  PackedFloat2_128
  select(const PackedInts_128 &mask, const PackedFloat2_128 &onTrue, const PackedFloat2_128 &onFalse) noexcept {
    // C-like truthiness: zero = false, non-zero = true
    return blendMasked(onTrue, onFalse, _mm_cmpeq_epi64(mask, Zero<PackedInts_128>()));
  }

  PackedFloat2_128
  select(const PackedFloat2_128 &mask, const PackedFloat2_128 &onTrue, const PackedFloat2_128 &onFalse) noexcept {
    // C-like truthiness: zero = false, non-zero = true
    return blendMasked(onTrue, onFalse, _mm_cmpeq_pd(mask, Zero<PackedFloat2_128>()));
  }
}
