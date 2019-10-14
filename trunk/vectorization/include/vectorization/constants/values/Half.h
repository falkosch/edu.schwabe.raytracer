#pragma once

#include "../../architecture.h"

namespace vectorization
{
    template <typename T>
    const T Half() noexcept;

    template <>
    const Float_32 Half<Float_32>() noexcept;

    template <>
    const Float_64 Half<Float_64>() noexcept;

    template <>
    const PackedFloat4_128 Half<PackedFloat4_128>() noexcept;

    template <>
    const PackedFloat2_128 Half<PackedFloat2_128>() noexcept;

    template <>
    const PackedFloat8_256 Half<PackedFloat8_256>() noexcept;

    template <>
    const PackedFloat4_256 Half<PackedFloat4_256>() noexcept;
}
