#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  PackedFloat2_128 select(bool mask, const PackedFloat2_128 &onTrue, const PackedFloat2_128 &onFalse) noexcept;

  PackedFloat2_128
  select(const PackedInts_128 &mask, const PackedFloat2_128 &onTrue, const PackedFloat2_128 &onFalse) noexcept;

  PackedFloat2_128
  select(const PackedFloat2_128 &mask, const PackedFloat2_128 &onTrue, const PackedFloat2_128 &onFalse) noexcept;
}
