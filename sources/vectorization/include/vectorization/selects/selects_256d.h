#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  PackedFloat4_256 select(bool mask, const PackedFloat4_256 &onTrue, const PackedFloat4_256 &onFalse) noexcept;

  PackedFloat4_256
  select(const PackedInts_256 &mask, const PackedFloat4_256 &onTrue, const PackedFloat4_256 &onFalse) noexcept;

  PackedFloat4_256
  select(const PackedFloat4_256 &mask, const PackedFloat4_256 &onTrue, const PackedFloat4_256 &onFalse) noexcept;
}
