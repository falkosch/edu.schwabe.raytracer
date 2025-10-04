#include "vectorization/selects/selects_256s.h"

#include "vectorization/blends.h"
#include "vectorization/constants.h"

namespace vectorization {
  const PackedFloat8_256
  select(const bool mask, const PackedFloat8_256 &onTrue, const PackedFloat8_256 &onFalse) noexcept {
    return mask ? onTrue : onFalse;
  }

  const PackedFloat8_256
  select(const PackedInts_256 &mask, const PackedFloat8_256 &onTrue, const PackedFloat8_256 &onFalse) noexcept {
    // reverse onTrue & onFalse since we test for mask components to be zero
    return blendMasked(onTrue, onFalse, _mm256_cmpeq_epi32(mask, Zero<PackedInts_256>()));
  }

  const PackedFloat8_256
  select(const PackedFloat8_256 &mask, const PackedFloat8_256 &onTrue, const PackedFloat8_256 &onFalse) noexcept {
    // reverse onTrue & onFalse since we test for mask components to be zero
    return blendMasked(onTrue, onFalse, _mm256_cmp_ps(mask, Zero<PackedFloat8_256>(), _CMP_EQ_OQ));
  }
}