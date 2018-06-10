#pragma once

#include "../architecture.h"

namespace vectorization
{

    template <typename V>
    const V MaskAll() noexcept;

    template <typename V>
    const V MaskX() noexcept;

    template <typename V>
    const V MaskXY() noexcept;

    template <typename V>
    const V MaskXYZ() noexcept;

    template <typename V>
    const V MaskXYZW() noexcept;

    template <typename V>
    const V MaskY() noexcept;

    template <typename V>
    const V MaskYZ() noexcept;

    template <typename V>
    const V MaskYZW() noexcept;

    template <typename V>
    const V MaskZ() noexcept;

    template <typename V>
    const V MaskZW() noexcept;

    template <typename V>
    const V MaskW() noexcept;

    template <>
    const PackedFloat4_128 MaskW<PackedFloat4_128>() noexcept;

}
