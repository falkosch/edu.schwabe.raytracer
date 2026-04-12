#include "vectorization/selects/selects_128i.h"

namespace vectorization {
  PackedInts_128 select(const bool mask, const PackedInts_128 &onTrue, const PackedInts_128 &onFalse) noexcept {
    return mask ? onTrue : onFalse;
  }

  PackedInts_128
  select(const PackedInts_128 &mask, const PackedInts_128 &onTrue, const PackedInts_128 &onFalse) noexcept {
    return _mm_blendv_epi8(onFalse, onTrue, mask);
  }
}
