#include "vectorization/functions/reciprocal.h"

#include "vectorization/accessors.h"
#include "vectorization/constants.h"

namespace vectorization
{
    const Float_32 reciprocal(const Float_32 v) noexcept {
#ifdef VECTORIZATION_APPROXIMATIONS
#ifdef VECTORIZATION_FINE_APPROXIMATIONS
        // Do two Newton-Raphson steps for y = 1/x
        // y_n+1 = y_n(2 - x * y_n)
        // y_0 = rcp(x)
        // y_1 = y_0(2 - x * y_0)
        // y_2 = y_1(2 - x * y_1)
        const PackedFloat4_128 two = Two<PackedFloat4_128>();
        const PackedFloat4_128 N = _mm_set_ss(v);
        const PackedFloat4_128 x0 = _mm_rcp_ss(N);
        const PackedFloat4_128 x1 = _mm_mul_ss(_mm_sub_ss(two, _mm_mul_ss(N, x0)), x0);
        return x(_mm_mul_ss(_mm_sub_ss(two, _mm_mul_ss(N, x1)), x1));
#else
        return x(_mm_rcp_ss(_mm_set_ss(v)));
#endif
#else
        return x(_mm_div_ss(One<PackedFloat4_128>(), _mm_set_ss(v)));
#endif
    }

    const Float_64 reciprocal(const Float_64 v) noexcept {
        // no approximative variants available in SSE or AVX
        return x(_mm_div_sd(One<PackedFloat2_128>(), _mm_set_sd(v)));
    }

    const PackedFloat4_128 reciprocal(const PackedFloat4_128 & v) noexcept {
#ifdef VECTORIZATION_APPROXIMATIONS
#ifdef VECTORIZATION_FINE_APPROXIMATIONS
        // Do two Newton-Raphson steps for y = 1/x
        // y_n+1 = y_n(2 - x * y_n)
        // y_0 = rcp(x)
        // y_1 = y_0(2 - x * y_0)
        // y_2 = y_1(2 - x * y_1)
        const PackedFloat4_128 two = Two<PackedFloat4_128>();
        const PackedFloat4_128 x0 = _mm_rcp_ps(v);
        const PackedFloat4_128 x1 = _mm_mul_ps(_mm_sub_ps(two, _mm_mul_ps(v, x0)), x0);
        return _mm_mul_ps(_mm_sub_ps(two, _mm_mul_ps(v, x1)), x1);
#else
        return _mm_rcp_ps(v);
#endif
#else
        return _mm_div_ps(One<PackedFloat4_128>(), v);
#endif
    }

    const PackedFloat2_128 reciprocal(const PackedFloat2_128 & v) noexcept {
        // no approximative variants available in SSE or AVX
        return _mm_div_pd(One<PackedFloat2_128>(), v);
    }
}