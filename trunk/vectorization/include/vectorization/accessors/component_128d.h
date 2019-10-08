#pragma once

#include "../architecture.h"

namespace vectorization
{
    template <ASizeT Index>
    const Float_64 component(const PackedFloat2_128 & v) noexcept;

    template <>
    const Float_64 component<VectorIndices::X>(const PackedFloat2_128 & v) noexcept;

    template <>
    const Float_64 component<VectorIndices::Y>(const PackedFloat2_128 & v) noexcept;

    const Float_64 x(const PackedFloat2_128 & v) noexcept;
    const Float_64 y(const PackedFloat2_128 & v) noexcept;

    const Float_64 x1(const PackedFloat2_128 & v) noexcept;
    const Float_64 x2(const PackedFloat2_128 & v) noexcept;
}
