#pragma once

#include "vectorization/architecture.h"

namespace vectorization
{
    template <typename T>
    const T Infinity() noexcept;

    template <>
    const Float_32 Infinity<Float_32>() noexcept;

    template <>
    const Float_64 Infinity<Float_64>() noexcept;

    template <>
    const PackedFloat4_128 Infinity<PackedFloat4_128>() noexcept;

    template <>
    const PackedFloat2_128 Infinity<PackedFloat2_128>() noexcept;

    template <>
    const PackedFloat8_256 Infinity<PackedFloat8_256>() noexcept;

    template <>
    const PackedFloat4_256 Infinity<PackedFloat4_256>() noexcept;
}
