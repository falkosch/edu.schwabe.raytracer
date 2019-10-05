#include "vectorization/constants/masks/MaskZW.h"

#include "vectorization/constants/masks/MaskAll.h"

namespace vectorization
{
    template <>
    const PackedFloat4_128 MaskZW<PackedFloat4_128>() noexcept {
        return _mm_castsi128_ps(_mm_slli_si128(MaskAll<PackedInts_128>(), VectorSizes::Y * sizeof(Float_32)));
    }
}