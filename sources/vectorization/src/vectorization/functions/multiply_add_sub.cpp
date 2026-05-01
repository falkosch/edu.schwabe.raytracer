#include "vectorization/functions/multiply_add_sub.h"

#include "vectorization/functions/add_subtract.h"
#include "vectorization/functions/multiply.h"

namespace vectorization {
  PackedFloat4_128 multiplyAddSub(const PackedFloat4_128 &a, const PackedFloat4_128 &b, const PackedFloat4_128 &c) noexcept {
#if VECTORIZATION_FMA
    return _mm_fmaddsub_ps(a, b, c);
#else
    return addSubtract(multiply(a, b), c);
#endif
  }

  PackedFloat2_128 multiplyAddSub(const PackedFloat2_128 &a, const PackedFloat2_128 &b, const PackedFloat2_128 &c) noexcept {
#if VECTORIZATION_FMA
    return _mm_fmaddsub_pd(a, b, c);
#else
    return addSubtract(multiply(a, b), c);
#endif
  }

  PackedFloat8_256 multiplyAddSub(const PackedFloat8_256 &a, const PackedFloat8_256 &b, const PackedFloat8_256 &c) noexcept {
#if VECTORIZATION_FMA
    return _mm256_fmaddsub_ps(a, b, c);
#else
    return addSubtract(multiply(a, b), c);
#endif
  }

  PackedFloat4_256 multiplyAddSub(const PackedFloat4_256 &a, const PackedFloat4_256 &b, const PackedFloat4_256 &c) noexcept {
#if VECTORIZATION_FMA
    return _mm256_fmaddsub_pd(a, b, c);
#else
    return addSubtract(multiply(a, b), c);
#endif
  }
}
