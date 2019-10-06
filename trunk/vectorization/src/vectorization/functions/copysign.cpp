#include "vectorization/functions/copysign.h"

#include "vectorization/constants/values/NegativeZero.h"
#include "vectorization/constants/values/One.h"

namespace vectorization
{
    const PackedFloat4_128 copysign(const PackedFloat4_128 & v) noexcept {
        return _mm_or_ps(One<PackedFloat4_128>(), _mm_and_ps(NegativeZero<PackedFloat4_128>(), v));
    }

    const PackedFloat2_128 copysign(const PackedFloat2_128 & v) noexcept {
        return _mm_or_pd(One<PackedFloat2_128>(), _mm_and_pd(NegativeZero<PackedFloat2_128>(), v));
    }

    const Float_32 copysign(const Float_32 v) noexcept {
        return _mm_cvtss_f32(copysign(_mm_set_ss(v)));
    }

    const Float_64 copysign(const Float_64 v) noexcept {
        return _mm_cvtsd_f64(copysign(_mm_set_sd(v)));
    }
}