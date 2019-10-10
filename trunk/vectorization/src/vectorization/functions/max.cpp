#include "vectorization/functions/max.h"

#include "vectorization/selects.h"

namespace vectorization
{
    const bool max(const bool a, const bool b) noexcept {
        return a | b;
    }

    const Int_8 max(const Int_8 a, const Int_8 b) noexcept {
        return select(a > b, a, b);
    }

    const UInt_8 max(const UInt_8 a, const UInt_8 b) noexcept {
        return select(a > b, a, b);
    }

    const Int_16 max(const Int_16 a, const Int_16 b) noexcept {
        return select(a > b, a, b);
    }

    const UInt_16 max(const UInt_16 a, const UInt_16 b) noexcept {
        return select(a > b, a, b);
    }

    const Int_32 max(const Int_32 a, const Int_32 b) noexcept {
        return select(a > b, a, b);
    }

    const UInt_32 max(const UInt_32 a, const UInt_32 b) noexcept {
        return select(a > b, a, b);
    }

    const Int_64 max(const Int_64 a, const Int_64 b) noexcept {
        return select(a > b, a, b);
    }

    const UInt_64 max(const UInt_64 a, const UInt_64 b) noexcept {
        return select(a > b, a, b);
    }

    const Float_32 max(const Float_32 a, const Float_32 b) noexcept {
        return _mm_cvtss_f32(
            _mm_max_ss(_mm_set_ss(a), _mm_set_ss(b))
        );
    }

    const Float_64 max(const Float_64 a, const Float_64 b) noexcept {
        return _mm_cvtsd_f64(
            _mm_max_sd(_mm_set_sd(a), _mm_set_sd(b))
        );
    }

    const PackedFloat4_128 max(const PackedFloat4_128 & a, const PackedFloat4_128 & b) noexcept {
        return _mm_max_ps(a, b);
    }

    const PackedFloat2_128 max(const PackedFloat2_128 & a, const PackedFloat2_128 & b) noexcept {
        return _mm_max_pd(a, b);
    }
}