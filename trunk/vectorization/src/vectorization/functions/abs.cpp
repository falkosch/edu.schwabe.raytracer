#include "vectorization/functions/abs.h"

#include "vectorization/constants/values/Zero.h"
#include "vectorization/constants/values/NegativeZero.h"
#include "vectorization/selects/selects.h"

namespace vectorization
{
    const Int_8 abs(const Int_8 v) noexcept {
        return select(v < Zero<Int_8>(), static_cast<Int_8>(-v), v);
    }

    const Int_16 abs(const Int_16 v) noexcept {
        return select(v < Zero<Int_16>(), static_cast<Int_16>(-v), v);
    }

    const Int_32 abs(const Int_32 v) noexcept {
        return std::abs(v);
    }

    const Int_64 abs(const Int_64 v) noexcept {
        return std::llabs(v);
    }

    const Float_32 abs(const Float_32 v) noexcept {
        return _mm_cvtss_f32(abs(_mm_set_ss(v)));
    }

    const Float_64 abs(const Float_64 v) noexcept {
        return _mm_cvtsd_f64(abs(_mm_set_sd(v)));
    }

    const PackedFloat4_128 abs(const PackedFloat4_128 & v) noexcept {
        return _mm_andnot_ps(NegativeZero<PackedFloat4_128>(), v);
    }

    const PackedFloat2_128 abs(const PackedFloat2_128 & v) noexcept {
        return _mm_andnot_pd(NegativeZero<PackedFloat2_128>(), v);
    }
}