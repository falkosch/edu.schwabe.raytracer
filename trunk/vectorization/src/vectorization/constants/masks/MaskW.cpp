#include "vectorization/constants/masks/MaskW.h"

#include "vectorization/constants/masks/MaskAll.h"

namespace vectorization
{
    template <>
    const PackedFloat4_128 MaskW<PackedFloat4_128>() noexcept {
        return _mm_castsi128_ps(_mm_slli_si128(MaskAll<PackedInts_128>(), VectorSizes::Z * sizeof(Float_32)));
    }
}