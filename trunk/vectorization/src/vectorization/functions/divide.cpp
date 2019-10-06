#include "vectorization/functions/divide.h"

namespace vectorization
{
    const Int_8 divide(const Int_8 a, const Int_8 b) noexcept {
        return static_cast<Int_8>(a / b);
    }

    const UInt_8 divide(const UInt_8 a, const UInt_8 b) noexcept {
        return static_cast<UInt_8>(a / b);
    }

    const Int_16 divide(const Int_16 a, const Int_16 b) noexcept {
        return static_cast<Int_16>(a / b);
    }

    const UInt_16 divide(const UInt_16 a, const UInt_16 b) noexcept {
        return static_cast<UInt_16>(a / b);
    }

    const Int_32 divide(const Int_32 a, const Int_32 b) noexcept {
        return a / b;
    }

    const UInt_32 divide(const UInt_32 a, const UInt_32 b) noexcept {
        return a / b;
    }

    const Int_64 divide(const Int_64 a, const Int_64 b) noexcept {
        return a / b;
    }

    const UInt_64 divide(const UInt_64 a, const UInt_64 b) noexcept {
        return a / b;
    }

    const Float_32 divide(const Float_32 a, const Float_32 b) noexcept {
#ifdef VECTORIZATION_APPROXIMATIONS
#ifdef VECTORIZATION_FINE_APPROXIMATIONS
        const PackedFloat4_128 two = Two<PackedFloat4_128>();
        const PackedFloat4_128 bs = _mm_set_ss(b);
        const PackedFloat4_128 x0 = _mm_rcp_ss(bs);
        const PackedFloat4_128 x1 = _mm_mul_ss(_mm_sub_ss(two, _mm_mul_ss(bs, x0)), x0);
        return _mm_cvtss_f32(_mm_mul_ss(_mm_set_ss(a), _mm_mul_ss(_mm_sub_ss(two, _mm_mul_ss(bs, x1)), x1)));
#else
        return _mm_cvtss_f32(_mm_mul_ss(_mm_set_ss(a), _mm_rcp_ss(_mm_set_ss(b))));
#endif
#else
        return _mm_cvtss_f32(_mm_div_ss(_mm_set_ss(a), _mm_set_ss(b)));
#endif
    }

    const Float_64 divide(const Float_64 a, const Float_64 b) noexcept {
        return a / b;
    }

    const PackedFloat4_128 divide(const PackedFloat4_128 & a, const PackedFloat4_128 & b) noexcept {
#ifdef VECTORIZATION_APPROXIMATIONS
#ifdef VECTORIZATION_FINE_APPROXIMATIONS
        const PackedFloat4_128 two = Two<PackedFloat4_128>();
        const PackedFloat4_128 x0 = _mm_rcp_ps(b);
        const PackedFloat4_128 x1 = _mm_mul_ps(_mm_sub_ps(two, _mm_mul_ps(b, x0)), x0);
        return _mm_mul_ps(a, _mm_mul_ps(_mm_sub_ps(two, _mm_mul_ps(b, x1)), x1));
#else
        return _mm_mul_ps(a, _mm_rcp_ps(b));
#endif
#else
        return _mm_div_ps(a, b);
#endif
    }

    const PackedFloat2_128 divide(const PackedFloat2_128 & a, const PackedFloat2_128 & b) noexcept {
        return _mm_div_pd(a, b);
    }
}