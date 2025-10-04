#include "vectorization/blends/blend_128d.h"

namespace vectorization {
  PackedFloat2_128 blendMasked(
      const PackedFloat2_128 &onBitNotSet, const PackedFloat2_128 &onBitSet, const PackedFloat2_128 &mask
  ) noexcept {
    return _mm_blendv_pd(onBitNotSet, onBitSet, mask);
  }

  PackedFloat2_128 blendMasked(
      const PackedFloat2_128 &onBitNotSet, const PackedFloat2_128 &onBitSet, const PackedInts_128 &mask
  ) noexcept {
    return blendMasked(onBitNotSet, onBitSet, _mm_castsi128_pd(mask));
  }
}
