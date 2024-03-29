#include "vectorization/functions/rsqrt.h"

#include "vectorization/functions/reciprocal.h"
#include "vectorization/functions/sqrt.h"

#include "vectorization/accessors.h"

namespace vectorization {
  const Float_32 rsqrt(const Float_32 v) noexcept {
#ifdef VECTORIZATION_APPROXIMATIONS
#ifdef VECTORIZATION_FINE_APPROXIMATIONS
    // Do two Newton-Raphson steps for y = 1/sqrt(x)
    // y_n+1 = y_n*(3/2 - x*y_n*y_n/2)
    // y_0 = rsqrt(x)
    // y_1 = y_0(3/2 - x * y_0 * y_0 / 2)
    // y_2 = y_1(3/2 - x * y_1 * y_1 / 2)
    const PackedFloat4_128 half = Half<PackedFloat4_128>();
    const PackedFloat4_128 oneHalf = OneHalf<PackedFloat4_128>();
    const PackedFloat4_128 N = _mm_set_ss(v);
    const PackedFloat4_128 x0 = _mm_rsqrt_ss(N);
    const PackedFloat4_128 x1 =
        _mm_mul_ss(_mm_sub_ss(oneHalf, _mm_mul_ss(half, _mm_mul_ss(N, _mm_mul_ss(x0, x0)))), x0);
    return x(_mm_mul_ss(_mm_sub_ss(oneHalf, _mm_mul_ss(half, _mm_mul_ss(N, _mm_mul_ss(x1, x1)))), x1));
#else
    return x(_mm_rsqrt_ss(_mm_set_ss(v)));
#endif
#else
    return reciprocal(sqrt(v));
#endif
  }

  const Float_64 rsqrt(const Float_64 v) noexcept {
    return reciprocal(sqrt(v));
  }

  const PackedFloat4_128 rsqrt(const PackedFloat4_128 &v) noexcept {
#ifdef VECTORIZATION_APPROXIMATIONS
#ifdef VECTORIZATION_FINE_APPROXIMATIONS
    const PackedFloat4_128 half = Half<PackedFloat4_128>();
    const PackedFloat4_128 oneHalf = OneHalf<PackedFloat4_128>();
    const PackedFloat4_128 x0 = _mm_rsqrt_ps(v);
    const PackedFloat4_128 x1 =
        _mm_mul_ps(_mm_sub_ps(oneHalf, _mm_mul_ps(half, _mm_mul_ps(v, _mm_mul_ps(x0, x0)))), x0);
    return _mm_mul_ps(_mm_sub_ps(oneHalf, _mm_mul_ps(half, _mm_mul_ps(v, _mm_mul_ps(x1, x1)))), x1);
#else
    return _mm_rsqrt_ps(v);
#endif
#else
    return reciprocal(sqrt(v));
#endif
  }

  const PackedFloat2_128 rsqrt(const PackedFloat2_128 &v) noexcept {
    return reciprocal(sqrt(v));
  }
}