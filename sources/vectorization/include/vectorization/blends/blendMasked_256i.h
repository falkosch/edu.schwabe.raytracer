#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  PackedInts_256
  blendMasked(const PackedInts_256 &onBitNotSet, const PackedInts_256 &onBitSet, const PackedInts_256 &mask) noexcept;
}
