#pragma once

#include "vectorization/architecture.h"

namespace vectorization
{
    template <typename V>
    const V MaskZW() noexcept;

    template <>
    const PackedFloat4_128 MaskZW<PackedFloat4_128>() noexcept;

    template <>
    const PackedFloat8_256 MaskZW<PackedFloat8_256>() noexcept;

    template <>
    const PackedFloat4_256 MaskZW<PackedFloat4_256>() noexcept;
}
