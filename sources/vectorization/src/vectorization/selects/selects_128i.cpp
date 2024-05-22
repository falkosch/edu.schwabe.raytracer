#include "vectorization/selects/selects_128i.h"

namespace vectorization {
  PackedInts_128 select(const bool mask, const PackedInts_128 &onTrue, const PackedInts_128 &onFalse) noexcept {
    return mask ? onTrue : onFalse;
  }
}
