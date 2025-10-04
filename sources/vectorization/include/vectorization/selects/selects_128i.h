#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  const PackedInts_128 select(const bool mask, const PackedInts_128 &onTrue, const PackedInts_128 &onFalse) noexcept;
}
