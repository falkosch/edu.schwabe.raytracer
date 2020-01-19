#pragma once

#include "vectorization/architecture.h"

namespace vectorization
{
    template <typename T>
    const T NegativeInfinity() noexcept;

    template <>
    const Float_32 NegativeInfinity<Float_32>() noexcept;

    template <>
    const Float_64 NegativeInfinity<Float_64>() noexcept;

    template <>
    const PackedFloat4_128 NegativeInfinity<PackedFloat4_128>() noexcept;

    template <>
    const PackedFloat2_128 NegativeInfinity<PackedFloat2_128>() noexcept;

    template <>
    const PackedFloat8_256 NegativeInfinity<PackedFloat8_256>() noexcept;

    template <>
    const PackedFloat4_256 NegativeInfinity<PackedFloat4_256>() noexcept;
}
