#include "vectorization/selects/selects_256d.h"

#include "vectorization/blends.h"
#include "vectorization/constants.h"

namespace vectorization
{
    const PackedFloat4_256 select(const bool mask, const PackedFloat4_256 & onTrue, const PackedFloat4_256 & onFalse) noexcept {
        return mask ? onTrue : onFalse;
    }

    const PackedFloat4_256 select(const PackedInts_256 & mask, const PackedFloat4_256 & onTrue, const PackedFloat4_256 & onFalse) noexcept {
        // reverse onTrue & onFalse since we test for mask components to be zero
        return blendMasked(onTrue, onFalse, _mm256_cmpeq_epi64(mask, Zero<PackedInts_256>()));
    }

    const PackedFloat4_256 select(const PackedFloat4_256 & mask, const PackedFloat4_256 & onTrue, const PackedFloat4_256 & onFalse) noexcept {
        // reverse onTrue & onFalse since we test for mask components to be zero
        return blendMasked(onTrue, onFalse, _mm256_cmp_pd(mask, Zero<PackedFloat4_256>(), _CMP_EQ_OQ));
    }
}