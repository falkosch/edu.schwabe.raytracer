#pragma once

#include "../architecture.h"

namespace vectorization
{
    template <ASizeT Index>
    const Float_64 component(const PackedFloat4_256 & v) noexcept;

    template <>
    const Float_64 component<VectorIndices::X>(const PackedFloat4_256 & v) noexcept;

    template <>
    const Float_64 component<VectorIndices::Y>(const PackedFloat4_256 & v) noexcept;

    template <>
    const Float_64 component<VectorIndices::Z>(const PackedFloat4_256 & v) noexcept;

    template <>
    const Float_64 component<VectorIndices::W>(const PackedFloat4_256 & v) noexcept;

    const Float_64 x(const PackedFloat4_256 & v) noexcept;
    const Float_64 y(const PackedFloat4_256 & v) noexcept;
    const Float_64 z(const PackedFloat4_256 & v) noexcept;
    const Float_64 w(const PackedFloat4_256 & v) noexcept;

    const Float_64 x1(const PackedFloat4_256 & v) noexcept;
    const Float_64 x2(const PackedFloat4_256 & v) noexcept;
    const Float_64 x3(const PackedFloat4_256 & v) noexcept;
    const Float_64 x4(const PackedFloat4_256 & v) noexcept;
}
