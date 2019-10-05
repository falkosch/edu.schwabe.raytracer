#include "vectorization/constants/values/OneX.h"

#include "vectorization/constants/values/One.h"

namespace vectorization
{
    template <>
    const PackedFloat4_128 OneX<PackedFloat4_128>() noexcept {
        return _mm_castsi128_ps(
            _mm_srli_si128(
                _mm_castps_si128(One<PackedFloat4_128>()),
                VectorSizes::Z * sizeof(Float_32)
            )
        );
    }

    template <>
    const PackedFloat2_128 OneX<PackedFloat2_128>() noexcept {
        return _mm_castsi128_pd(
            _mm_srli_si128(
                _mm_castpd_si128(One<PackedFloat2_128>()),
                VectorSizes::X * sizeof(Float_64)
            )
        );
    }
}