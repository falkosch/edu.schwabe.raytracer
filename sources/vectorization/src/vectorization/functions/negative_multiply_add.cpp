#include "vectorization/functions/negative_multiply_add.h"

#include "vectorization/functions/multiply.h"
#include "vectorization/functions/subtract.h"

namespace vectorization {
  PackedFloat4_128
  negativeMultiplyAdd(const PackedFloat4_128 &a, const PackedFloat4_128 &b, const PackedFloat4_128 &c) noexcept {
#if VECTORIZATION_FMA
    return _mm_fnmadd_ps(a, b, c);
#else
    return subtract(c, multiply(a, b));
#endif
  }

  PackedFloat2_128
  negativeMultiplyAdd(const PackedFloat2_128 &a, const PackedFloat2_128 &b, const PackedFloat2_128 &c) noexcept {
#if VECTORIZATION_FMA
    return _mm_fnmadd_pd(a, b, c);
#else
    return subtract(c, multiply(a, b));
#endif
  }

  PackedFloat8_256
  negativeMultiplyAdd(const PackedFloat8_256 &a, const PackedFloat8_256 &b, const PackedFloat8_256 &c) noexcept {
#if VECTORIZATION_FMA
    return _mm256_fnmadd_ps(a, b, c);
#else
    return subtract(c, multiply(a, b));
#endif
  }

  PackedFloat4_256
  negativeMultiplyAdd(const PackedFloat4_256 &a, const PackedFloat4_256 &b, const PackedFloat4_256 &c) noexcept {
#if VECTORIZATION_FMA
    return _mm256_fnmadd_pd(a, b, c);
#else
    return subtract(c, multiply(a, b));
#endif
  }

  PackedFloat4_128
  negativeMultiplyAddX(const PackedFloat4_128 &a, const PackedFloat4_128 &b, const PackedFloat4_128 &c) noexcept {
#if VECTORIZATION_FMA
    return _mm_fnmadd_ss(a, b, c);
#else
    // _mm_fnmadd_ss returns {-(a[0]*b[0]) + c[0], a[1], a[2], a[3]}.
    // subtractX/multiplyX cannot compose this correctly because subtractX
    // passes through yzw from its first argument. Use _mm_move_ss to take
    // x from the packed result and yzw from a.
    return _mm_move_ss(a, subtract(c, multiply(a, b)));
#endif
  }

  PackedFloat2_128
  negativeMultiplyAddX(const PackedFloat2_128 &a, const PackedFloat2_128 &b, const PackedFloat2_128 &c) noexcept {
#if VECTORIZATION_FMA
    return _mm_fnmadd_sd(a, b, c);
#else
    return _mm_move_sd(a, subtract(c, multiply(a, b)));
#endif
  }
}
