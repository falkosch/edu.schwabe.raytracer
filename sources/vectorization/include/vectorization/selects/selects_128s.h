#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  PackedFloat4_128 select(bool mask, const PackedFloat4_128 &onTrue, const PackedFloat4_128 &onFalse) noexcept;

  PackedFloat4_128
  select(const PackedInts_128 &mask, const PackedFloat4_128 &onTrue, const PackedFloat4_128 &onFalse) noexcept;

  PackedFloat4_128
  select(const PackedFloat4_128 &mask, const PackedFloat4_128 &onTrue, const PackedFloat4_128 &onFalse) noexcept;
}
