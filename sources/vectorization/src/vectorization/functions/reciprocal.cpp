#include "vectorization/functions/reciprocal.h"

#include "vectorization/accessors/component_128d.h"
#include "vectorization/accessors/component_128s.h"
#include "vectorization/constants/values.h"
#include "vectorization/functions/multiply.h"
#include "vectorization/functions/negative_multiply_add.h"

namespace vectorization {
  Float_32 reciprocal(const Float_32 value) noexcept {
#ifdef VECTORIZATION_APPROXIMATIONS
#ifdef VECTORIZATION_FINE_APPROXIMATIONS
    // Do two Newton-Raphson steps for y = 1/x
    // y_n+1 = y_n(2 - x * y_n)
    // y_0 = rcp(x)
    // y_1 = y_0(2 - x * y_0)
    // y_2 = y_1(2 - x * y_1)
    const auto two = Two<PackedFloat4_128>();
    const auto N = _mm_set_ss(value);
    const auto x0 = _mm_rcp_ss(N);
    const auto x1 = multiplyX(negativeMultiplyAddX(N, x0, two), x0);
    return x(multiplyX(negativeMultiplyAddX(N, x1, two), x1));
#else
    return x(_mm_rcp_ss(_mm_set_ss(value)));
#endif
#else
    return x(_mm_div_ss(One<PackedFloat4_128>(), _mm_set_ss(value)));
#endif
  }

  Float_64 reciprocal(const Float_64 value) noexcept {
    // no approximation available for 64-bit floats in SSE or AVX
    return x(_mm_div_sd(One<PackedFloat2_128>(), _mm_set_sd(value)));
  }

  PackedFloat4_128 reciprocal(const PackedFloat4_128 &values) noexcept {
#ifdef VECTORIZATION_APPROXIMATIONS
#ifdef VECTORIZATION_FINE_APPROXIMATIONS
    // Do two Newton-Raphson steps for y = 1/x
    // y_n+1 = y_n(2 - x * y_n)
    // y_0 = rcp(x)
    // y_1 = y_0(2 - x * y_0)
    // y_2 = y_1(2 - x * y_1)
    const auto two = Two<PackedFloat4_128>();
    const auto x0 = _mm_rcp_ps(values);
    const auto x1 = multiply(negativeMultiplyAdd(values, x0, two), x0);
    return multiply(negativeMultiplyAdd(values, x1, two), x1);
#else
    return _mm_rcp_ps(values);
#endif
#else
    return _mm_div_ps(One<PackedFloat4_128>(), values);
#endif
  }

  PackedFloat2_128 reciprocal(const PackedFloat2_128 &values) noexcept {
    // double-packed rcp-op not available in SSE or AVX
    return _mm_div_pd(One<PackedFloat2_128>(), values);
  }
}
