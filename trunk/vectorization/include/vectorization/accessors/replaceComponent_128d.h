#pragma once

#include "vectorization/architecture.h"

namespace vectorization
{
    template <ASizeT Index>
    const PackedFloat2_128 replaceComponent(const PackedFloat2_128 & v, const Float_64 s) noexcept;

    template <>
    const PackedFloat2_128 replaceComponent<VectorIndices::X>(const PackedFloat2_128 & v, const Float_64 s) noexcept;

    template <>
    const PackedFloat2_128 replaceComponent<VectorIndices::Y>(const PackedFloat2_128 & v, const Float_64 s) noexcept;

    const PackedFloat2_128 replaceX(const PackedFloat2_128 & v, const Float_64 s) noexcept;
    const PackedFloat2_128 replaceY(const PackedFloat2_128 & v, const Float_64 s) noexcept;

    const PackedFloat2_128 replaceX1(const PackedFloat2_128 & v, const Float_64 s) noexcept;
    const PackedFloat2_128 replaceX2(const PackedFloat2_128 & v, const Float_64 s) noexcept;

    const PackedFloat2_128 replaceComponent(const PackedFloat2_128 & v, const PackedFloat2_128 & replacement, const ASizeT index) noexcept;
    const PackedFloat2_128 replaceComponent(const PackedFloat2_128 & v, const Float_64 replacement, const ASizeT index) noexcept;
}
