#include "vectorization/constants/values/SelfOcclusionEpsilon.h"

#include "vectorization/accessors.h"

namespace vectorization
{
#define SELF_OCCLUSION_EPSILON 0.001953125

    const PackedFloat2_128 SelfOcclusionEpsilon_PackedFloat2_128 = _mm_set1_pd(Float_64{ SELF_OCCLUSION_EPSILON });
    const PackedFloat4_128 SelfOcclusionEpsilon_PackedFloat4_128 = _mm_set1_ps(Float_32{ SELF_OCCLUSION_EPSILON });
    const PackedFloat4_256 SelfOcclusionEpsilon_PackedFloat4_256 = _mm256_set1_pd(Float_64{ SELF_OCCLUSION_EPSILON });
    const PackedFloat8_256 SelfOcclusionEpsilon_PackedFloat8_256 = _mm256_set1_ps(Float_32{ SELF_OCCLUSION_EPSILON });

    template <>
    const Float_32 SelfOcclusionEpsilon<Float_32>() noexcept {
        return x(SelfOcclusionEpsilon<PackedFloat4_128>());
    }

    template <>
    const Float_64 SelfOcclusionEpsilon<Float_64>() noexcept {
        return x(SelfOcclusionEpsilon<PackedFloat2_128>());
    }

    template <>
    const PackedFloat2_128 SelfOcclusionEpsilon<PackedFloat2_128>() noexcept {
        return SelfOcclusionEpsilon_PackedFloat2_128;
    }

    template <>
    const PackedFloat4_128 SelfOcclusionEpsilon<PackedFloat4_128>() noexcept {
        return SelfOcclusionEpsilon_PackedFloat4_128;
    }

    template <>
    const PackedFloat4_256 SelfOcclusionEpsilon<PackedFloat4_256>() noexcept {
        return SelfOcclusionEpsilon_PackedFloat4_256;
    }

    template <>
    const PackedFloat8_256 SelfOcclusionEpsilon<PackedFloat8_256>() noexcept {
        return SelfOcclusionEpsilon_PackedFloat8_256;
    }
}