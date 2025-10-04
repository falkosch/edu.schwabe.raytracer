#include "vectorization/blends/blendMasked_256i.h"

namespace vectorization {
  const PackedInts_256
  blendMasked(const PackedInts_256 &onBitNotSet, const PackedInts_256 &onBitSet, const PackedInts_256 &mask) noexcept {
    return _mm256_blendv_epi8(onBitNotSet, onBitSet, mask);
  }
}