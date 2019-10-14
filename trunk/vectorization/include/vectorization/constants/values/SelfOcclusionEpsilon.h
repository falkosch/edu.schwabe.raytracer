#pragma once

#include "../../architecture.h"

namespace vectorization
{
    template <typename T>
    const T SelfOcclusionEpsilon() noexcept;

    template <>
    const Float_32 SelfOcclusionEpsilon<Float_32>() noexcept;

    template <>
    const Float_64 SelfOcclusionEpsilon<Float_64>() noexcept;

    template <>
    const PackedFloat4_128 SelfOcclusionEpsilon<PackedFloat4_128>() noexcept;

    template <>
    const PackedFloat2_128 SelfOcclusionEpsilon<PackedFloat2_128>() noexcept;

    template <>
    const PackedFloat8_256 SelfOcclusionEpsilon<PackedFloat8_256>() noexcept;

    template <>
    const PackedFloat4_256 SelfOcclusionEpsilon<PackedFloat4_256>() noexcept;
}
