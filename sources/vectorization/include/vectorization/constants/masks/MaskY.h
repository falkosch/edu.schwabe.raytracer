#pragma once

#include "vectorization/architecture.h"

namespace vectorization
{
    template <typename V>
    const V MaskY() noexcept;

    template <>
    const PackedFloat4_128 MaskY<PackedFloat4_128>() noexcept;

    template <>
    const PackedFloat2_128 MaskY<PackedFloat2_128>() noexcept;

    template <>
    const PackedFloat8_256 MaskY<PackedFloat8_256>() noexcept;

    template <>
    const PackedFloat4_256 MaskY<PackedFloat4_256>() noexcept;
}
