#pragma once

#include "../../architecture.h"

#define C_PI 3.1415926535897932384626433832795

namespace vectorization
{
    template <typename T>
    const T Pi() noexcept;

    template <>
    const Float_32 Pi<Float_32>() noexcept;

    template <>
    const Float_64 Pi<Float_64>() noexcept;

    template <>
    const PackedFloat4_128 Pi<PackedFloat4_128>() noexcept;

    template <>
    const PackedFloat2_128 Pi<PackedFloat2_128>() noexcept;

    template <>
    const PackedFloat8_256 Pi<PackedFloat8_256>() noexcept;

    template <>
    const PackedFloat4_256 Pi<PackedFloat4_256>() noexcept;
}
