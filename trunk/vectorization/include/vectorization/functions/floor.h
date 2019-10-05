#pragma once

#include "../architecture/compiler_intrinsics.h"

namespace vectorization
{
    const Float_32 floor(const Float_32 v) noexcept;

    const Float_64 floor(const Float_64 v) noexcept;

    const PackedFloat4_128 floor(const PackedFloat4_128 & v) noexcept;

    const PackedFloat2_128 floor(const PackedFloat2_128 & v) noexcept;

    const PackedFloat8_256 floor(const PackedFloat8_256 & v) noexcept;

    const PackedFloat4_256 floor(const PackedFloat4_256 & v) noexcept;
}
