#include "vectorization/v_ui32_8/functions.h"

#include "vectorization/v_ui32_8/constants.h"
#include "vectorization/v_ui32_8/operators.h"

namespace vectorization {
  v_ui32_8 andnot(const v_ui32_8 &a, const v_ui32_8 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_andnot_si256(a.components, b.components);
#elif VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    const auto lo = _mm_andnot_si128(_mm256_castsi256_si128(a.components), _mm256_castsi256_si128(b.components));
    const auto hi = _mm_andnot_si128(_mm256_extractf128_si256(a.components, 1), _mm256_extractf128_si256(b.components, 1));
    return _mm256_insertf128_si256(_mm256_castsi128_si256(lo), hi, 1);
#else
    return {{_mm_andnot_si128(a.components.lo, b.components.lo),
             _mm_andnot_si128(a.components.hi, b.components.hi)}};
#endif
  }

  v_ui32_8 min(const v_ui32_8 &a, const v_ui32_8 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_min_epu32(a.components, b.components);
#elif VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    const auto lo = _mm_min_epu32(_mm256_castsi256_si128(a.components), _mm256_castsi256_si128(b.components));
    const auto hi = _mm_min_epu32(_mm256_extractf128_si256(a.components, 1), _mm256_extractf128_si256(b.components, 1));
    return _mm256_insertf128_si256(_mm256_castsi128_si256(lo), hi, 1);
#else
    return {{_mm_min_epu32(a.components.lo, b.components.lo), _mm_min_epu32(a.components.hi, b.components.hi)}};
#endif
  }

  v_ui32_8 max(const v_ui32_8 &a, const v_ui32_8 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_max_epu32(a.components, b.components);
#elif VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    const auto lo = _mm_max_epu32(_mm256_castsi256_si128(a.components), _mm256_castsi256_si128(b.components));
    const auto hi = _mm_max_epu32(_mm256_extractf128_si256(a.components, 1), _mm256_extractf128_si256(b.components, 1));
    return _mm256_insertf128_si256(_mm256_castsi128_si256(lo), hi, 1);
#else
    return {{_mm_max_epu32(a.components.lo, b.components.lo), _mm_max_epu32(a.components.hi, b.components.hi)}};
#endif
  }

  v_ui32_8 clamp(const v_ui32_8 &v, const v_ui32_8 &l, const v_ui32_8 &m) noexcept {
    return max(l, min(v, m));
  }

  bool anyTrue(const v_ui32_8 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return !_mm256_testz_si256(v.components, v.components);
#else
    return !(_mm_testz_si128(v.components.lo, v.components.lo)
          && _mm_testz_si128(v.components.hi, v.components.hi));
#endif
  }

  bool anyFalse(const v_ui32_8 &v) noexcept {
    return !allTrue(v);
  }

  bool allTrue(const v_ui32_8 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_movemask_epi8(_mm256_cmpeq_epi32(v.components, Zero<v_ui32_8::PackedType>())) == 0;
#elif VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    const auto z = Zero<PackedInts_128>();
    return _mm_movemask_epi8(_mm_cmpeq_epi32(_mm256_castsi256_si128(v.components), z)) == 0
        && _mm_movemask_epi8(_mm_cmpeq_epi32(_mm256_extractf128_si256(v.components, 1), z)) == 0;
#else
    const auto z = Zero<v_ui32_8::HalfPackedType>();
    return _mm_movemask_epi8(_mm_cmpeq_epi32(v.components.lo, z)) == 0
        && _mm_movemask_epi8(_mm_cmpeq_epi32(v.components.hi, z)) == 0;
#endif
  }

  bool allFalse(const v_ui32_8 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return _mm256_testz_si256(v.components, v.components);
#else
    return _mm_testz_si128(v.components.lo, v.components.lo)
        && _mm_testz_si128(v.components.hi, v.components.hi);
#endif
  }
}
