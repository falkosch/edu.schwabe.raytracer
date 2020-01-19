#include "vectorization/selects/selects_128s.h"

#include "vectorization/blends.h"
#include "vectorization/constants.h"

namespace vectorization
{
    const PackedFloat4_128 select(const bool mask, const PackedFloat4_128 & onTrue, const PackedFloat4_128 & onFalse) noexcept {
        return mask ? onTrue : onFalse;
    }

    const PackedFloat4_128 select(const PackedInts_128 & mask, const PackedFloat4_128 & onTrue, const PackedFloat4_128 & onFalse) noexcept {
        // reverse onTrue & onFalse since we test for mask components to be zero
        return blendMasked(onTrue, onFalse, _mm_cmpeq_epi32(mask, Zero<PackedInts_128>()));
    }

    const PackedFloat4_128 select(const PackedFloat4_128 & mask, const PackedFloat4_128 & onTrue, const PackedFloat4_128 & onFalse) noexcept {
        // reverse onTrue & onFalse since we test for mask components to be zero
        return blendMasked(onTrue, onFalse, _mm_cmpeq_ps(mask, Zero<PackedFloat4_128>()));
    }
}