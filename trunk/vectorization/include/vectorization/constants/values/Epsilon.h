#pragma once

#include "../../architecture.h"

namespace vectorization
{
    template <typename T>
    const T Epsilon() noexcept;

    template <>
    const Float_32 Epsilon<Float_32>() noexcept;

    template <>
    const Float_64 Epsilon<Float_64>() noexcept;

    template <>
    const PackedFloat4_128 Epsilon<PackedFloat4_128>() noexcept;

    template <>
    const PackedFloat2_128 Epsilon<PackedFloat2_128>() noexcept;

    template <>
    const PackedFloat8_256 Epsilon<PackedFloat8_256>() noexcept;

    template <>
    const PackedFloat4_256 Epsilon<PackedFloat4_256>() noexcept;
}
