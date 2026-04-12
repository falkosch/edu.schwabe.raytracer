#include "vectorization/selects/selects_128s.h"


namespace vectorization {
  PackedFloat4_128 select(const bool mask, const PackedFloat4_128 &onTrue, const PackedFloat4_128 &onFalse) noexcept {
    return mask ? onTrue : onFalse;
  }

  PackedFloat4_128
  select(const PackedInts_128 &mask, const PackedFloat4_128 &onTrue, const PackedFloat4_128 &onFalse) noexcept {
    return _mm_blendv_ps(onFalse, onTrue, _mm_castsi128_ps(mask));
  }

  PackedFloat4_128
  select(const PackedFloat4_128 &mask, const PackedFloat4_128 &onTrue, const PackedFloat4_128 &onFalse) noexcept {
    return _mm_blendv_ps(onFalse, onTrue, mask);
  }
}
