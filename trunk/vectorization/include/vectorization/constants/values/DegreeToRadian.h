#pragma once

#include "../../architecture.h"

namespace vectorization
{
    template <typename T>
    const T DegreeToRadian() noexcept;

    template <>
    const Float_32 DegreeToRadian<Float_32>() noexcept;

    template <>
    const Float_64 DegreeToRadian<Float_64>() noexcept;

    template <>
    const PackedFloat4_128 DegreeToRadian<PackedFloat4_128>() noexcept;

    template <>
    const PackedFloat2_128 DegreeToRadian<PackedFloat2_128>() noexcept;

    template <>
    const PackedFloat8_256 DegreeToRadian<PackedFloat8_256>() noexcept;

    template <>
    const PackedFloat4_256 DegreeToRadian<PackedFloat4_256>() noexcept;
}
