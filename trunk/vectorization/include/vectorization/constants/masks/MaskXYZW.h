#pragma once

#include "../../architecture.h"

namespace vectorization
{
    template <typename V>
    const V MaskXYZW() noexcept;

    template <>
    const PackedFloat4_128 MaskXYZW<PackedFloat4_128>() noexcept;

    template <>
    const PackedFloat8_256 MaskXYZW<PackedFloat8_256>() noexcept;

    template <>
    const PackedFloat4_256 MaskXYZW<PackedFloat4_256>() noexcept;
}
