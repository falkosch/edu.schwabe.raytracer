#include "vectorization/functions/isNaN.h"

namespace vectorization
{
    const bool isNaN(const Float_32 v) noexcept {
        const PackedFloat4_128 t = _mm_set_ss(v);
        return !!_mm_cvtsi128_si32(
            _mm_castps_si128(_mm_cmpunord_ss(t, t))
        );
    }

    const bool isNaN(const Float_32 a, const Float_32 b) noexcept {
        return !!_mm_cvtsi128_si32(
            _mm_castps_si128(
                _mm_cmpunord_ss(_mm_set_ss(a), _mm_set_ss(b))
            )
        );
    }

    const bool isNaN(const Float_64 v) noexcept {
        const PackedFloat2_128 t = _mm_set_sd(v);
        return !!_mm_cvtsi128_si32(
            _mm_castpd_si128(_mm_cmpunord_sd(t, t))
        );
    }

    const bool isNaN(const Float_64 a, const Float_64 b) noexcept {
        return !!_mm_cvtsi128_si32(
            _mm_castpd_si128(
                _mm_cmpunord_sd(_mm_set_sd(a), _mm_set_sd(b))
            )
        );
    }

    const PackedFloat4_128 isNaN(const PackedFloat4_128 & v) noexcept {
        return _mm_cmpunord_ps(v, v);
    }

    const PackedFloat4_128 isNaN(const PackedFloat4_128 & a, const PackedFloat4_128 & b) noexcept {
        return _mm_cmpunord_ps(a, b);
    }

    const PackedFloat2_128 isNaN(const PackedFloat2_128 & v) noexcept {
        return _mm_cmpunord_pd(v, v);
    }

    const PackedFloat2_128 isNaN(const PackedFloat2_128 & a, const PackedFloat2_128 & b) noexcept {
        return _mm_cmpunord_pd(a, b);
    }
}