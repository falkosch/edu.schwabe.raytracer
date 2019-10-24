#pragma once

#include "vectorization/architecture.h"

namespace vectorization
{
    template <typename T>
    const T Sin45() noexcept;

    template <>
    const Float_32 Sin45<Float_32>() noexcept;

    template <>
    const Float_64 Sin45<Float_64>() noexcept;

    template <>
    const PackedFloat4_128 Sin45<PackedFloat4_128>() noexcept;

    template <>
    const PackedFloat2_128 Sin45<PackedFloat2_128>() noexcept;

    template <>
    const PackedFloat8_256 Sin45<PackedFloat8_256>() noexcept;

    template <>
    const PackedFloat4_256 Sin45<PackedFloat4_256>() noexcept;
}
