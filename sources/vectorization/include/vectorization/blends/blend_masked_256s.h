#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  PackedFloat8_256 blendMasked(
      const PackedFloat8_256 &onBitNotSet, const PackedFloat8_256 &onBitSet, const PackedFloat8_256 &mask
  ) noexcept;

  PackedFloat8_256 blendMasked(
      const PackedFloat8_256 &onBitNotSet, const PackedFloat8_256 &onBitSet, const PackedInts_256 &mask
  ) noexcept;
}
