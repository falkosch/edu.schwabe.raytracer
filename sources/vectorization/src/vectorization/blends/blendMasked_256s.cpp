#include "vectorization/blends/blend_256s.h"

namespace vectorization {
  const PackedFloat8_256 blendMasked(
      const PackedFloat8_256 &onBitNotSet, const PackedFloat8_256 &onBitSet, const PackedFloat8_256 &mask
  ) noexcept {
    return _mm256_blendv_ps(onBitNotSet, onBitSet, mask);
  }

  const PackedFloat8_256 blendMasked(
      const PackedFloat8_256 &onBitNotSet, const PackedFloat8_256 &onBitSet, const PackedInts_256 &mask
  ) noexcept {
    return blendMasked(onBitNotSet, onBitSet, _mm256_castsi256_ps(mask));
  }
}