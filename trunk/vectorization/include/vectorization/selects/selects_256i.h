#pragma once

#include "vectorization/architecture.h"

namespace vectorization
{
    const PackedInts_256 select(const bool mask, const PackedInts_256 & onTrue, const PackedInts_256 & onFalse) noexcept;
}
