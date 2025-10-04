#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  PackedFloat4_256 blendMasked(
      const PackedFloat4_256 &onBitNotSet, const PackedFloat4_256 &onBitSet, const PackedFloat4_256 &mask
  ) noexcept;

  PackedFloat4_256 blendMasked(
      const PackedFloat4_256 &onBitNotSet, const PackedFloat4_256 &onBitSet, const PackedInts_256 &mask
  ) noexcept;
}
