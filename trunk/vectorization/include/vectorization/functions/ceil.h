#pragma once

#include "../architecture/compiler_intrinsics.h"

namespace vectorization
{
    const Float_32 ceil(const Float_32 v) noexcept;

    const Float_64 ceil(const Float_64 v) noexcept;

    const PackedFloat4_128 ceil(const PackedFloat4_128 & v) noexcept;

    const PackedFloat2_128 ceil(const PackedFloat2_128 & v) noexcept;

    const PackedFloat8_256 ceil(const PackedFloat8_256 & v) noexcept;

    const PackedFloat4_256 ceil(const PackedFloat4_256 & v) noexcept;
}
