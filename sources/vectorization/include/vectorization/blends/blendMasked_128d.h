#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  PackedFloat2_128 blendMasked(
      const PackedFloat2_128 &onBitNotSet, const PackedFloat2_128 &onBitSet, const PackedFloat2_128 &mask
  ) noexcept;

  PackedFloat2_128 blendMasked(
      const PackedFloat2_128 &onBitNotSet, const PackedFloat2_128 &onBitSet, const PackedInts_128 &mask
  ) noexcept;
}
