#include "vectorization/constants/values/RadianToUniform.h"

#include "vectorization/constants/values/Pi.h"

#define RADIAN_TO_UNIFORM 0.5 / C_PI

namespace vectorization
{
    const PackedFloat4_128 RadianToUniform_PackedFloat4_128 = _mm_set_ps1(static_cast<Float_32>(RADIAN_TO_UNIFORM));
    const PackedFloat2_128 RadianToUniform_PackedFloat2_128 = _mm_set1_pd(RADIAN_TO_UNIFORM);

    template <>
    const PackedFloat4_128 RadianToUniform<PackedFloat4_128>() noexcept {
        return RadianToUniform_PackedFloat4_128;
    }

    template <>
    const PackedFloat2_128 RadianToUniform<PackedFloat2_128>() noexcept {
        return RadianToUniform_PackedFloat2_128;
    }

    template <>
    const Float_32 RadianToUniform<Float_32>() noexcept {
        return _mm_cvtss_f32(RadianToUniform<PackedFloat4_128>());
    }

    template <>
    const Float_64 RadianToUniform<Float_64>() noexcept {
        return _mm_cvtsd_f64(RadianToUniform<PackedFloat2_128>());
    }
}