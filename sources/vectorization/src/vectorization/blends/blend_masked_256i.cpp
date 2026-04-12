#include "vectorization/blends/blend_masked_256i.h"

namespace vectorization {
  PackedInts_256
  blendMasked(const PackedInts_256 &onBitNotSet, const PackedInts_256 &onBitSet, const PackedInts_256 &mask) noexcept {
    return _mm256_blendv_epi8(onBitNotSet, onBitSet, mask);
  }
}
