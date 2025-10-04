#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  const PackedFloat2_128 blendMasked(
      const PackedFloat2_128 &onBitNotSet, const PackedFloat2_128 &onBitSet, const PackedFloat2_128 &mask
  ) noexcept;

  const PackedFloat2_128 blendMasked(
      const PackedFloat2_128 &onBitNotSet, const PackedFloat2_128 &onBitSet, const PackedInts_128 &mask
  ) noexcept;
}
