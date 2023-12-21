#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  const PackedFloat4_128 blendMasked(
      const PackedFloat4_128 &onBitNotSet, const PackedFloat4_128 &onBitSet, const PackedFloat4_128 &mask
  ) noexcept;

  const PackedFloat4_128 blendMasked(
      const PackedFloat4_128 &onBitNotSet, const PackedFloat4_128 &onBitSet, const PackedInts_128 &mask
  ) noexcept;
}
