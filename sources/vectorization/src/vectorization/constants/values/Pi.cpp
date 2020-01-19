#include "vectorization/constants/values/Pi.h"

#include "vectorization/accessors.h"

namespace vectorization
{
    const PackedFloat2_128 Pi_PackedFloat2_128 = _mm_set1_pd(Float_64{ C_PI });
    const PackedFloat4_128 Pi_PackedFloat4_128 = _mm_set1_ps(Float_32{ C_PI });
    const PackedFloat4_256 Pi_PackedFloat4_256 = _mm256_set1_pd(Float_64{ C_PI });
    const PackedFloat8_256 Pi_PackedFloat8_256 = _mm256_set1_ps(Float_32{ C_PI });

    template <>
    const Float_32 Pi<Float_32>() noexcept {
        return x(Pi<PackedFloat4_128>());
    }

    template <>
    const Float_64 Pi<Float_64>() noexcept {
        return x(Pi<PackedFloat2_128>());
    }

    template <>
    const PackedFloat2_128 Pi<PackedFloat2_128>() noexcept {
        return Pi_PackedFloat2_128;
    }

    template <>
    const PackedFloat4_128 Pi<PackedFloat4_128>() noexcept {
        return Pi_PackedFloat4_128;
    }

    template <>
    const PackedFloat4_256 Pi<PackedFloat4_256>() noexcept {
        return Pi_PackedFloat4_256;
    }

    template <>
    const PackedFloat8_256 Pi<PackedFloat8_256>() noexcept {
        return Pi_PackedFloat8_256;
    }
}