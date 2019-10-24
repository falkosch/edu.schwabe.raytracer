#pragma once

#include "vectorization/architecture.h"

namespace vectorization
{
    const PackedFloat2_128 select(const bool mask, const PackedFloat2_128 & onTrue, const PackedFloat2_128 & onFalse) noexcept;

    const PackedFloat2_128 select(const PackedInts_128 & mask, const PackedFloat2_128 & onTrue, const PackedFloat2_128 & onFalse) noexcept;

    const PackedFloat2_128 select(const PackedFloat2_128 & mask, const PackedFloat2_128 & onTrue, const PackedFloat2_128 & onFalse) noexcept;
}
