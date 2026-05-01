#include "vectorization/functions/multiply_add.h"

#include "vectorization/functions/add.h"
#include "vectorization/functions/multiply.h"

namespace vectorization {
  PackedFloat4_128 multiplyAdd(const PackedFloat4_128 &a, const PackedFloat4_128 &b, const PackedFloat4_128 &c) noexcept {
#if VECTORIZATION_FMA
    return _mm_fmadd_ps(a, b, c);
#else
    return add(multiply(a, b), c);
#endif
  }

  PackedFloat2_128 multiplyAdd(const PackedFloat2_128 &a, const PackedFloat2_128 &b, const PackedFloat2_128 &c) noexcept {
#if VECTORIZATION_FMA
    return _mm_fmadd_pd(a, b, c);
#else
    return add(multiply(a, b), c);
#endif
  }

  PackedFloat8_256 multiplyAdd(const PackedFloat8_256 &a, const PackedFloat8_256 &b, const PackedFloat8_256 &c) noexcept {
#if VECTORIZATION_FMA
    return _mm256_fmadd_ps(a, b, c);
#else
    return add(multiply(a, b), c);
#endif
  }

  PackedFloat4_256 multiplyAdd(const PackedFloat4_256 &a, const PackedFloat4_256 &b, const PackedFloat4_256 &c) noexcept {
#if VECTORIZATION_FMA
    return _mm256_fmadd_pd(a, b, c);
#else
    return add(multiply(a, b), c);
#endif
  }

  PackedFloat4_128 multiplyAddX(const PackedFloat4_128 &a, const PackedFloat4_128 &b, const PackedFloat4_128 &c) noexcept {
#if VECTORIZATION_FMA
    return _mm_fmadd_ss(a, b, c);
#else
    return addX(multiplyX(a, b), c);
#endif
  }

  PackedFloat2_128 multiplyAddX(const PackedFloat2_128 &a, const PackedFloat2_128 &b, const PackedFloat2_128 &c) noexcept {
#if VECTORIZATION_FMA
    return _mm_fmadd_sd(a, b, c);
#else
    return addX(multiplyX(a, b), c);
#endif
  }
}
