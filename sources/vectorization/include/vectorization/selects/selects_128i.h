#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  PackedInts_128 select(bool mask, const PackedInts_128 &onTrue, const PackedInts_128 &onFalse) noexcept;
}
