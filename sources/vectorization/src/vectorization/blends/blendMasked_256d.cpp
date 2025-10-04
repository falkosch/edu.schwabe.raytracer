#include "vectorization/blends/blend_256d.h"

namespace vectorization {
  PackedFloat4_256 blendMasked(
      const PackedFloat4_256 &onBitNotSet, const PackedFloat4_256 &onBitSet, const PackedFloat4_256 &mask
  ) noexcept {
    return _mm256_blendv_pd(onBitNotSet, onBitSet, mask);
  }

  PackedFloat4_256 blendMasked(
      const PackedFloat4_256 &onBitNotSet, const PackedFloat4_256 &onBitSet, const PackedInts_256 &mask
  ) noexcept {
    return blendMasked(onBitNotSet, onBitSet, _mm256_castsi256_pd(mask));
  }
}
