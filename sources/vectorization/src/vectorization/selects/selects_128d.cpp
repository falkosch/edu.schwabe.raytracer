#include "vectorization/selects/selects_128d.h"

#include "vectorization/blends.h"
#include "vectorization/constants.h"

namespace vectorization {
  PackedFloat2_128 select(const bool mask, const PackedFloat2_128 &onTrue, const PackedFloat2_128 &onFalse) noexcept {
    return mask ? onTrue : onFalse;
  }

  PackedFloat2_128
  select(const PackedInts_128 &mask, const PackedFloat2_128 &onTrue, const PackedFloat2_128 &onFalse) noexcept {
    // reverse onTrue & onFalse since we test for mask components to be zero
    return blendMasked(onTrue, onFalse, _mm_cmpeq_epi64(mask, Zero<PackedInts_128>()));
  }

  PackedFloat2_128
  select(const PackedFloat2_128 &mask, const PackedFloat2_128 &onTrue, const PackedFloat2_128 &onFalse) noexcept {
    // reverse onTrue & onFalse since we test for mask components to be zero
    return blendMasked(onTrue, onFalse, _mm_cmpeq_pd(mask, Zero<PackedFloat2_128>()));
  }
}
