#include "vectorization/functions/round.h"

#include "vectorization/constants/values/Zero.h"

namespace vectorization
{
    const Float_32 round(const Float_32 v) noexcept {
        return _mm_cvtss_f32(
            _mm_round_ss(Zero<PackedFloat4_128>(), _mm_set_ss(v), _MM_FROUND_TO_NEAREST_INT)
        );
    }

    const Float_64 round(const Float_64 v) noexcept {
        return _mm_cvtsd_f64(
            _mm_round_sd(Zero<PackedFloat2_128>(), _mm_set_sd(v), _MM_FROUND_TO_NEAREST_INT)
        );
    }

    const PackedFloat4_128 round(const PackedFloat4_128 & v) noexcept {
        return _mm_round_ps(v, _MM_FROUND_TO_NEAREST_INT);
    }

    const PackedFloat2_128 round(const PackedFloat2_128 & v) noexcept {
        return _mm_round_pd(v, _MM_FROUND_TO_NEAREST_INT);
    }
}