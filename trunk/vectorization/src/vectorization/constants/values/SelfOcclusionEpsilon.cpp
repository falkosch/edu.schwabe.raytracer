#include "vectorization/constants/values/SelfOcclusionEpsilon.h"

#define SELF_OCCLUSION_EPSILON 0.001953125

namespace vectorization
{
    const PackedFloat4_128 SelfOcclusionEpsilon_PackedFloat4_128 = _mm_set_ps1(static_cast<Float_32>(SELF_OCCLUSION_EPSILON));
    const PackedFloat2_128 SelfOcclusionEpsilon_PackedFloat2_128 = _mm_set1_pd(SELF_OCCLUSION_EPSILON);

    template <>
    const PackedFloat4_128 SelfOcclusionEpsilon<PackedFloat4_128>() noexcept {
        return SelfOcclusionEpsilon_PackedFloat4_128;
    }

    template <>
    const PackedFloat2_128 SelfOcclusionEpsilon<PackedFloat2_128>() noexcept {
        return SelfOcclusionEpsilon_PackedFloat2_128;
    }

    template <>
    const Float_32 SelfOcclusionEpsilon<Float_32>() noexcept {
        return _mm_cvtss_f32(SelfOcclusionEpsilon<PackedFloat4_128>());
    }

    template <>
    const Float_64 SelfOcclusionEpsilon<Float_64>() noexcept {
        return _mm_cvtsd_f64(SelfOcclusionEpsilon<PackedFloat2_128>());
    }
}