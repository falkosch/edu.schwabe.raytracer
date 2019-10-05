#include "vectorization/constants/values/NegativeInfinity.h"

#include "vectorization/constants/masks/MaskAll.h"

namespace vectorization
{
    template <>
    const PackedFloat4_128 NegativeInfinity<PackedFloat4_128>() noexcept {
        // erase fraction part (23 bits)
        return _mm_castsi128_ps(_mm_slli_epi32(MaskAll<PackedInts_128>(), 23));
    }

    template <>
    const PackedFloat2_128 NegativeInfinity<PackedFloat2_128>() noexcept {
        // erase double fraction part (52 bits)
        return _mm_castsi128_pd(_mm_slli_epi64(MaskAll<PackedInts_128>(), 52));
    }

    template <>
    const Float_32 NegativeInfinity<Float_32>() noexcept {
        return _mm_cvtss_f32(NegativeInfinity<PackedFloat4_128>());
    }

    template <>
    const Float_64 NegativeInfinity<Float_64>() noexcept {
        return _mm_cvtsd_f64(NegativeInfinity<PackedFloat2_128>());
    }
}