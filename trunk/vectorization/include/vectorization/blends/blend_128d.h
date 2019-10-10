#pragma once

#include "../architecture.h"

namespace vectorization
{
    template <bool SelectX, bool SelectY>
    // generic blend of two vectors
    const PackedFloat2_128 blend(const PackedFloat2_128 & a, const PackedFloat2_128 & b) noexcept;

    template <>
    // passtrough a
    const PackedFloat2_128 blend<false, false>(const PackedFloat2_128 & a, const PackedFloat2_128 & b) noexcept;

    template <>
    // passthrough b
    const PackedFloat2_128 blend<true, true>(const PackedFloat2_128 & a, const PackedFloat2_128 & b) noexcept;

    template <>
    // x(a), y(b)
    const PackedFloat2_128 blend<false, true>(const PackedFloat2_128 & a, const PackedFloat2_128 & b) noexcept;

    template <>
    // x(b), y(a)
    const PackedFloat2_128 blend<true, false>(const PackedFloat2_128 & a, const PackedFloat2_128 & b) noexcept;
}
