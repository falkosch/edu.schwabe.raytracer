#pragma once

#include "vectorization/architecture.h"

namespace vectorization
{
    template <typename T>
    const T OneHalf() noexcept;

    template <>
    const Float_32 OneHalf<Float_32>() noexcept;

    template <>
    const Float_64 OneHalf<Float_64>() noexcept;

    template <>
    const PackedFloat4_128 OneHalf<PackedFloat4_128>() noexcept;

    template <>
    const PackedFloat2_128 OneHalf<PackedFloat2_128>() noexcept;

    template <>
    const PackedFloat8_256 OneHalf<PackedFloat8_256>() noexcept;

    template <>
    const PackedFloat4_256 OneHalf<PackedFloat4_256>() noexcept;
}
