#include "vectorization/constants/masks/MaskY.h"

#include "vectorization/constants/masks/MaskAll.h"
#include "vectorization/constants/masks/MaskX.h"

namespace vectorization
{
    template <>
    const PackedFloat4_128 MaskY<PackedFloat4_128>() noexcept {
        return _mm_castsi128_ps(
            _mm_slli_si128(
                _mm_castps_si128(MaskX<PackedFloat4_128>()),
                VectorSizes::X * sizeof(Float_32)
            )
        );
    }

    template <>
    const PackedFloat2_128 MaskY<PackedFloat2_128>() noexcept {
        return _mm_castsi128_pd(_mm_slli_si128(MaskAll<PackedInts_128>(), VectorSizes::X * sizeof(Float_64)));
    }
}