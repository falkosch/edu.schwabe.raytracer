#pragma once

#include "vectorization/architecture.h"

namespace vectorization
{
    template <ASizeT Index>
    const Float_32 component(const PackedFloat4_128 & v) noexcept;

    template <>
    const Float_32 component<VectorIndices::X>(const PackedFloat4_128 & v) noexcept;

    template <>
    const Float_32 component<VectorIndices::Y>(const PackedFloat4_128 & v) noexcept;

    template <>
    const Float_32 component<VectorIndices::Z>(const PackedFloat4_128 & v) noexcept;

    template <>
    const Float_32 component<VectorIndices::W>(const PackedFloat4_128 & v) noexcept;

    const Float_32 x(const PackedFloat4_128 & v) noexcept;
    const Float_32 y(const PackedFloat4_128 & v) noexcept;
    const Float_32 z(const PackedFloat4_128 & v) noexcept;
    const Float_32 w(const PackedFloat4_128 & v) noexcept;

    const Float_32 x1(const PackedFloat4_128 & v) noexcept;
    const Float_32 x2(const PackedFloat4_128 & v) noexcept;
    const Float_32 x3(const PackedFloat4_128 & v) noexcept;
    const Float_32 x4(const PackedFloat4_128 & v) noexcept;
}
