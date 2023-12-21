#include "vectorization/blends/blend_128s.h"

namespace vectorization {
  const PackedFloat4_128 blendMasked(
      const PackedFloat4_128 &onBitNotSet, const PackedFloat4_128 &onBitSet, const PackedFloat4_128 &mask
  ) noexcept {
    return _mm_blendv_ps(onBitNotSet, onBitSet, mask);
  }

  const PackedFloat4_128 blendMasked(
      const PackedFloat4_128 &onBitNotSet, const PackedFloat4_128 &onBitSet, const PackedInts_128 &mask
  ) noexcept {
    return blendMasked(onBitNotSet, onBitSet, _mm_castsi128_ps(mask));
  }
}