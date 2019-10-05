#include "vectorization/constants/values/Pi.h"

namespace vectorization
{
    const PackedFloat4_128 Pi_PackedFloat4_128 = _mm_set_ps1(static_cast<Float_32>(C_PI));
    const PackedFloat2_128 Pi_PackedFloat2_128 = _mm_set1_pd(C_PI);

    template <>
    const PackedFloat4_128 Pi<PackedFloat4_128>() noexcept {
        return Pi_PackedFloat4_128;
    }

    template <>
    const PackedFloat2_128 Pi<PackedFloat2_128>() noexcept {
        return Pi_PackedFloat2_128;
    }

    template <>
    const Float_32 Pi<Float_32>() noexcept {
        return _mm_cvtss_f32(Pi<PackedFloat4_128>());
    }

    template <>
    const Float_64 Pi<Float_64>() noexcept {
        return _mm_cvtsd_f64(Pi<PackedFloat2_128>());
    }
}