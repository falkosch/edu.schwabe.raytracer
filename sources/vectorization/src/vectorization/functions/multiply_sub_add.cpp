#include "vectorization/functions/multiply_sub_add.h"

#include "vectorization/functions/add_subtract.h"
#include "vectorization/functions/multiply.h"

#include "vectorization/constants/values.h"

namespace vectorization {
  PackedFloat4_128 multiplySubAdd(const PackedFloat4_128 &a, const PackedFloat4_128 &b, const PackedFloat4_128 &c) noexcept {
#if VECTORIZATION_FMA
    return _mm_fmsubadd_ps(a, b, c);
#else
    return addSubtract(multiply(a, b), _mm_xor_ps(NegativeZero<PackedFloat4_128>(), c));
#endif
  }

  PackedFloat2_128 multiplySubAdd(const PackedFloat2_128 &a, const PackedFloat2_128 &b, const PackedFloat2_128 &c) noexcept {
#if VECTORIZATION_FMA
    return _mm_fmsubadd_pd(a, b, c);
#else
    return addSubtract(multiply(a, b), _mm_xor_pd(NegativeZero<PackedFloat2_128>(), c));
#endif
  }

  PackedFloat8_256 multiplySubAdd(const PackedFloat8_256 &a, const PackedFloat8_256 &b, const PackedFloat8_256 &c) noexcept {
#if VECTORIZATION_FMA
    return _mm256_fmsubadd_ps(a, b, c);
#else
    return addSubtract(multiply(a, b), _mm256_xor_ps(NegativeZero<PackedFloat8_256>(), c));
#endif
  }

  PackedFloat4_256 multiplySubAdd(const PackedFloat4_256 &a, const PackedFloat4_256 &b, const PackedFloat4_256 &c) noexcept {
#if VECTORIZATION_FMA
    return _mm256_fmsubadd_pd(a, b, c);
#else
    return addSubtract(multiply(a, b), _mm256_xor_pd(NegativeZero<PackedFloat4_256>(), c));
#endif
  }
}
