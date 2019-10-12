#pragma once

#include "../architecture.h"

namespace vectorization
{
    const PackedFloat4_128 select(const bool mask, const PackedFloat4_128 & onTrue, const PackedFloat4_128 & onFalse) noexcept;

    const PackedFloat4_128 select(const PackedInts_128 & mask, const PackedFloat4_128 & onTrue, const PackedFloat4_128 & onFalse) noexcept;

    const PackedFloat4_128 select(const PackedFloat4_128 & mask, const PackedFloat4_128 & onTrue, const PackedFloat4_128 & onFalse) noexcept;
}
