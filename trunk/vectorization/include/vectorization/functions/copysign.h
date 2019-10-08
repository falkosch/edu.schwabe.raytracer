#pragma once

#include "../architecture.h"

namespace vectorization
{
    const Float_32 copysign(const Float_32 v) noexcept;

    const Float_64 copysign(const Float_64 v) noexcept;

    const PackedFloat4_128 copysign(const PackedFloat4_128 & v) noexcept;

    const PackedFloat2_128 copysign(const PackedFloat2_128 & v) noexcept;

    const PackedFloat8_256 copysign(const PackedFloat8_256 & v) noexcept;

    const PackedFloat4_256 copysign(const PackedFloat4_256 & v) noexcept;
}
