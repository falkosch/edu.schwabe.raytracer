#include "vectorization/functions/multiply_sub.h"

#include "vectorization/functions/multiply.h"
#include "vectorization/functions/subtract.h"

namespace vectorization {
  PackedFloat4_128 multiplySub(const PackedFloat4_128 &a, const PackedFloat4_128 &b, const PackedFloat4_128 &c) noexcept {
#if VECTORIZATION_FMA
    return _mm_fmsub_ps(a, b, c);
#else
    return subtract(multiply(a, b), c);
#endif
  }

  PackedFloat2_128 multiplySub(const PackedFloat2_128 &a, const PackedFloat2_128 &b, const PackedFloat2_128 &c) noexcept {
#if VECTORIZATION_FMA
    return _mm_fmsub_pd(a, b, c);
#else
    return subtract(multiply(a, b), c);
#endif
  }

  PackedFloat8_256 multiplySub(const PackedFloat8_256 &a, const PackedFloat8_256 &b, const PackedFloat8_256 &c) noexcept {
#if VECTORIZATION_FMA
    return _mm256_fmsub_ps(a, b, c);
#else
    return subtract(multiply(a, b), c);
#endif
  }

  PackedFloat4_256 multiplySub(const PackedFloat4_256 &a, const PackedFloat4_256 &b, const PackedFloat4_256 &c) noexcept {
#if VECTORIZATION_FMA
    return _mm256_fmsub_pd(a, b, c);
#else
    return subtract(multiply(a, b), c);
#endif
  }

  PackedFloat4_128 multiplySubX(const PackedFloat4_128 &a, const PackedFloat4_128 &b, const PackedFloat4_128 &c) noexcept {
#if VECTORIZATION_FMA
    return _mm_fmsub_ss(a, b, c);
#else
    return subtractX(multiplyX(a, b), c);
#endif
  }

  PackedFloat2_128 multiplySubX(const PackedFloat2_128 &a, const PackedFloat2_128 &b, const PackedFloat2_128 &c) noexcept {
#if VECTORIZATION_FMA
    return _mm_fmsub_sd(a, b, c);
#else
    return subtractX(multiplyX(a, b), c);
#endif
  }
}
