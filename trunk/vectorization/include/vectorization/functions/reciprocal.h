#pragma once

#include "vectorization/architecture.h"

namespace vectorization
{
    const Float_32 reciprocal(const Float_32 v) noexcept;

    const Float_64 reciprocal(const Float_64 v) noexcept;

    const PackedFloat4_128 reciprocal(const PackedFloat4_128 & v) noexcept;

    const PackedFloat2_128 reciprocal(const PackedFloat2_128 & v) noexcept;

    const PackedFloat8_256 reciprocal(const PackedFloat8_256 & v) noexcept;

    const PackedFloat4_256 reciprocal(const PackedFloat4_256 & v) noexcept;
}
