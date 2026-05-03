#include "vectorization/v_i64_4/swizzles.h"

namespace vectorization {
  template <>
  v_i64_4 swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(const v_i64_4 &v) noexcept {
    return v;
  }

  v_i64_4 xxxx(const v_i64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_permute4x64_epi64(v.components, _MM_SHUFFLE(0, 0, 0, 0));
#else
    const auto val = v[VectorIndices::X];
    return v_i64_4(val, val, val, val);
#endif
  }

  v_i64_4 yyyy(const v_i64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_permute4x64_epi64(v.components, _MM_SHUFFLE(1, 1, 1, 1));
#else
    const auto val = v[VectorIndices::Y];
    return v_i64_4(val, val, val, val);
#endif
  }

  v_i64_4 zzzz(const v_i64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_permute4x64_epi64(v.components, _MM_SHUFFLE(2, 2, 2, 2));
#else
    const auto val = v[VectorIndices::Z];
    return v_i64_4(val, val, val, val);
#endif
  }

  v_i64_4 wwww(const v_i64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_permute4x64_epi64(v.components, _MM_SHUFFLE(3, 3, 3, 3));
#else
    const auto val = v[VectorIndices::W];
    return v_i64_4(val, val, val, val);
#endif
  }
}