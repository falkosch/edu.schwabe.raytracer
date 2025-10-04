#include "vectorization/blends/blendMasked_128i.h"

namespace vectorization {
  const PackedInts_128
  blendMasked(const PackedInts_128 &onBitNotSet, const PackedInts_128 &onBitSet, const PackedInts_128 &mask) noexcept {
    return _mm_blendv_epi8(onBitNotSet, onBitSet, mask);
  }
}