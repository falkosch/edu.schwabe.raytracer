#include "vectorization/functions/negative_multiply_sub.h"

#include "vectorization/functions/multiply.h"
#include "vectorization/functions/subtract.h"

#include "vectorization/constants/values.h"

namespace vectorization {
  PackedFloat4_128
  negativeMultiplySub(const PackedFloat4_128 &a, const PackedFloat4_128 &b, const PackedFloat4_128 &c) noexcept {
#if VECTORIZATION_FMA
    return _mm_fnmsub_ps(a, b, c);
#else
    return subtract(_mm_xor_ps(NegativeZero<PackedFloat4_128>(), multiply(a, b)), c);
#endif
  }

  PackedFloat2_128
  negativeMultiplySub(const PackedFloat2_128 &a, const PackedFloat2_128 &b, const PackedFloat2_128 &c) noexcept {
#if VECTORIZATION_FMA
    return _mm_fnmsub_pd(a, b, c);
#else
    return subtract(_mm_xor_pd(NegativeZero<PackedFloat2_128>(), multiply(a, b)), c);
#endif
  }

  PackedFloat8_256
  negativeMultiplySub(const PackedFloat8_256 &a, const PackedFloat8_256 &b, const PackedFloat8_256 &c) noexcept {
#if VECTORIZATION_FMA
    return _mm256_fnmsub_ps(a, b, c);
#else
    return subtract(_mm256_xor_ps(NegativeZero<PackedFloat8_256>(), multiply(a, b)), c);
#endif
  }

  PackedFloat4_256
  negativeMultiplySub(const PackedFloat4_256 &a, const PackedFloat4_256 &b, const PackedFloat4_256 &c) noexcept {
#if VECTORIZATION_FMA
    return _mm256_fnmsub_pd(a, b, c);
#else
    return subtract(_mm256_xor_pd(NegativeZero<PackedFloat4_256>(), multiply(a, b)), c);
#endif
  }

  PackedFloat4_128
  negativeMultiplySubX(const PackedFloat4_128 &a, const PackedFloat4_128 &b, const PackedFloat4_128 &c) noexcept {
#if VECTORIZATION_FMA
    return _mm_fnmsub_ss(a, b, c);
#else
    // _mm_fnmsub_ss returns {-(a[0]*b[0]) - c[0], a[1], a[2], a[3]}.
    // Use _mm_move_ss to take x from the packed result and yzw from a.
    return _mm_move_ss(a, subtract(_mm_xor_ps(NegativeZero<PackedFloat4_128>(), multiply(a, b)), c));
#endif
  }

  PackedFloat2_128
  negativeMultiplySubX(const PackedFloat2_128 &a, const PackedFloat2_128 &b, const PackedFloat2_128 &c) noexcept {
#if VECTORIZATION_FMA
    return _mm_fnmsub_sd(a, b, c);
#else
    return _mm_move_sd(a, subtract(_mm_xor_pd(NegativeZero<PackedFloat2_128>(), multiply(a, b)), c));
#endif
  }
}
