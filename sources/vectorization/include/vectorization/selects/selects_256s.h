#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  PackedFloat8_256 select(bool mask, const PackedFloat8_256 &onTrue, const PackedFloat8_256 &onFalse) noexcept;

  PackedFloat8_256
  select(const PackedInts_256 &mask, const PackedFloat8_256 &onTrue, const PackedFloat8_256 &onFalse) noexcept;

  PackedFloat8_256
  select(const PackedFloat8_256 &mask, const PackedFloat8_256 &onTrue, const PackedFloat8_256 &onFalse) noexcept;
}
