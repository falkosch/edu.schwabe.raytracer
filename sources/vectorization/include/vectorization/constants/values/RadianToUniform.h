#pragma once

#include "vectorization/architecture.h"

namespace vectorization
{
    template <typename T>
    const T RadianToUniform() noexcept;

    template <>
    const Float_32 RadianToUniform<Float_32>() noexcept;

    template <>
    const Float_64 RadianToUniform<Float_64>() noexcept;

    template <>
    const PackedFloat4_128 RadianToUniform<PackedFloat4_128>() noexcept;

    template <>
    const PackedFloat2_128 RadianToUniform<PackedFloat2_128>() noexcept;

    template <>
    const PackedFloat8_256 RadianToUniform<PackedFloat8_256>() noexcept;

    template <>
    const PackedFloat4_256 RadianToUniform<PackedFloat4_256>() noexcept;
}
