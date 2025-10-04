#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  PackedInts_256 select(bool mask, const PackedInts_256 &onTrue, const PackedInts_256 &onFalse) noexcept;
}
