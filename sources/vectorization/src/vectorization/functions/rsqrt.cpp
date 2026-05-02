#include "vectorization/functions/rsqrt.h"

#include "vectorization/functions/multiply.h"
#include "vectorization/functions/negative_multiply_add.h"
#include "vectorization/functions/reciprocal.h"
#include "vectorization/functions/sqrt.h"

#ifdef VECTORIZATION_APPROXIMATIONS
#include "vectorization/accessors/component_128d.h"
#include "vectorization/accessors/component_128s.h"
#endif

namespace vectorization {
  Float_32 rsqrt(const Float_32 value) noexcept {
#ifdef VECTORIZATION_APPROXIMATIONS
#ifdef VECTORIZATION_FINE_APPROXIMATIONS
    // Do two Newton-Raphson steps for y = 1/sqrt(x)
    // y_n+1 = y_n*(3/2 - x*y_n*y_n/2)
    // y_0 = rsqrt(x)
    // y_1 = y_0(3/2 - x * y_0 * y_0 / 2)
    // y_2 = y_1(3/2 - x * y_1 * y_1 / 2)
    const auto half = Half<PackedFloat4_128>();
    const auto oneHalf = OneHalf<PackedFloat4_128>();
    const auto N = _mm_set_ss(value);
    const auto x0 = _mm_rsqrt_ss(N);
    const auto x1 = multiplyX(negativeMultiplyAddX(half, multiplyX(N, multiplyX(x0, x0)), oneHalf), x0);
    return x(multiplyX(negativeMultiplyAddX(half, multiplyX(N, multiplyX(x1, x1)), oneHalf), x1));
#else
    return x(_mm_rsqrt_ss(_mm_set_ss(value)));
#endif
#else
    return reciprocal(sqrt(value));
#endif
  }

  Float_64 rsqrt(const Float_64 value) noexcept {
    return reciprocal(sqrt(value));
  }

  PackedFloat4_128 rsqrt(const PackedFloat4_128 &values) noexcept {
#ifdef VECTORIZATION_APPROXIMATIONS
#ifdef VECTORIZATION_FINE_APPROXIMATIONS
    const auto half = Half<PackedFloat4_128>();
    const auto oneHalf = OneHalf<PackedFloat4_128>();
    const auto x0 = _mm_rsqrt_ps(values);
    const auto x1 = multiply(negativeMultiplyAdd(half, multiply(values, multiply(x0, x0)), oneHalf), x0);
    return multiply(negativeMultiplyAdd(half, multiply(values, multiply(x1, x1)), oneHalf), x1);
#else
    return _mm_rsqrt_ps(values);
#endif
#else
    return reciprocal(sqrt(values));
#endif
  }

  PackedFloat2_128 rsqrt(const PackedFloat2_128 &values) noexcept {
    return reciprocal(sqrt(values));
  }

  PackedFloat8_256 rsqrt(const PackedFloat8_256 &values) noexcept {
#ifdef VECTORIZATION_APPROXIMATIONS
#ifdef VECTORIZATION_FINE_APPROXIMATIONS
    const auto half = Half<PackedFloat8_256>();
    const auto oneHalf = OneHalf<PackedFloat8_256>();
    const auto x0 = _mm256_rsqrt_ps(values);
    const auto x1 = multiply(negativeMultiplyAdd(half, multiply(values, multiply(x0, x0)), oneHalf), x0);
    return multiply(negativeMultiplyAdd(half, multiply(values, multiply(x1, x1)), oneHalf), x1);
#else
    return _mm256_rsqrt_ps(values);
#endif
#else
    return reciprocal(sqrt(values));
#endif
  }

  PackedFloat4_256 rsqrt(const PackedFloat4_256 &values) noexcept {
    return reciprocal(sqrt(values));
  }
}
