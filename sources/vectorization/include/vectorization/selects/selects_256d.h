#pragma once

#include "vectorization/architecture.h"

namespace vectorization
{
    const PackedFloat4_256 select(const bool mask, const PackedFloat4_256 & onTrue, const PackedFloat4_256 & onFalse) noexcept;

    const PackedFloat4_256 select(const PackedInts_256 & mask, const PackedFloat4_256 & onTrue, const PackedFloat4_256 & onFalse) noexcept;

    const PackedFloat4_256 select(const PackedFloat4_256 & mask, const PackedFloat4_256 & onTrue, const PackedFloat4_256 & onFalse) noexcept;
}
