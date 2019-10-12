#pragma once

#include "../architecture.h"

namespace vectorization
{
    const PackedFloat8_256 select(const bool mask, const PackedFloat8_256 & onTrue, const PackedFloat8_256 & onFalse) noexcept;

    const PackedFloat8_256 select(const PackedInts_256 & mask, const PackedFloat8_256 & onTrue, const PackedFloat8_256 & onFalse) noexcept;

    const PackedFloat8_256 select(const PackedFloat8_256 & mask, const PackedFloat8_256 & onTrue, const PackedFloat8_256 & onFalse) noexcept;
}
