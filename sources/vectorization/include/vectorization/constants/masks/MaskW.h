#pragma once

#include "vectorization/architecture.h"

namespace vectorization
{
    template <typename V>
    const V MaskW() noexcept;

    template <>
    const PackedFloat4_128 MaskW<PackedFloat4_128>() noexcept;

    template <>
    const PackedFloat8_256 MaskW<PackedFloat8_256>() noexcept;

    template <>
    const PackedFloat4_256 MaskW<PackedFloat4_256>() noexcept;
}
