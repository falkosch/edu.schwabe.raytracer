#include "vectorization/selects/selects_128d.h"

namespace vectorization {
  PackedFloat2_128 select(const bool mask, const PackedFloat2_128 &onTrue, const PackedFloat2_128 &onFalse) noexcept {
    return mask ? onTrue : onFalse;
  }

  PackedFloat2_128
  select(const PackedInts_128 &mask, const PackedFloat2_128 &onTrue, const PackedFloat2_128 &onFalse) noexcept {
    return _mm_blendv_pd(onFalse, onTrue, _mm_castsi128_pd(mask));
  }

  PackedFloat2_128
  select(const PackedFloat2_128 &mask, const PackedFloat2_128 &onTrue, const PackedFloat2_128 &onFalse) noexcept {
    return _mm_blendv_pd(onFalse, onTrue, mask);
  }
}
