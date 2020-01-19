#pragma once

#include "vectorization/architecture.h"

namespace vectorization
{
    template <typename T>
    const T ReciprocalPi() noexcept;

    template <>
    const Float_32 ReciprocalPi<Float_32>() noexcept;

    template <>
    const Float_64 ReciprocalPi<Float_64>() noexcept;

    template <>
    const PackedFloat4_128 ReciprocalPi<PackedFloat4_128>() noexcept;

    template <>
    const PackedFloat2_128 ReciprocalPi<PackedFloat2_128>() noexcept;

    template <>
    const PackedFloat8_256 ReciprocalPi<PackedFloat8_256>() noexcept;

    template <>
    const PackedFloat4_256 ReciprocalPi<PackedFloat4_256>() noexcept;
}
