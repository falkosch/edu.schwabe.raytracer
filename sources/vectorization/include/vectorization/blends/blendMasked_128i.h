#pragma once

#include "vectorization/architecture.h"

namespace vectorization
{
    const PackedInts_128 blendMasked(const PackedInts_128 & onBitNotSet, const PackedInts_128 & onBitSet, const PackedInts_128 & mask) noexcept;
}
