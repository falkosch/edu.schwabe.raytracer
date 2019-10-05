#pragma once

#include "../architecture/compiler_intrinsics.h"

namespace vectorization
{
    const Float_32 rsqrt(const Float_32 v) noexcept;

    const Float_64 rsqrt(const Float_64 v) noexcept;

    const PackedFloat4_128 rsqrt(const PackedFloat4_128 & v) noexcept;

    const PackedFloat2_128 rsqrt(const PackedFloat2_128 & v) noexcept;

    const PackedFloat8_256 rsqrt(const PackedFloat8_256 & v) noexcept;

    const PackedFloat4_256 rsqrt(const PackedFloat4_256 & v) noexcept;
}
