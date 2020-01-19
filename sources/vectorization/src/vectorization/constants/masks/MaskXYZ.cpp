#include "vectorization/constants/masks/MaskXYZ.h"

#include "vectorization/constants/masks/MaskAll.h"
#include "vectorization/constants/masks/MaskNone.h"

namespace vectorization
{
    template <>
    const PackedFloat4_128 MaskXYZ<PackedFloat4_128>() noexcept {
        return _mm_castsi128_ps(
            _mm_srli_si128(MaskAll<PackedInts_128>(), VectorSizes::X * sizeof(Float_32))
        );
    }

    template <>
    const PackedFloat8_256 MaskXYZ<PackedFloat8_256>() noexcept {
        return _mm256_blend_ps(
            MaskNone<PackedFloat8_256>(),
            MaskAll<PackedFloat8_256>(),
            0b01110111
        );
    }

    template <>
    const PackedFloat4_256 MaskXYZ<PackedFloat4_256>() noexcept {
        return _mm256_blend_pd(
            MaskNone<PackedFloat4_256>(),
            MaskAll<PackedFloat4_256>(),
            0b0111
        );
    }
}