#include "vectorization/v_f32_8/swizzles.h"

#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
#include "vectorization/swizzles/swizzle_256s.h"
#endif

namespace vectorization {
  template <>
  v_f32_8 swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(const v_f32_8 &v) noexcept {
    return v;
  }

  v_f32_8 xxxx(const v_f32_8 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return xxxx(v.components);
#else
    return {{_mm_shuffle_ps(v.components.lo, v.components.lo, _MM_SHUFFLE(0, 0, 0, 0)),
             _mm_shuffle_ps(v.components.hi, v.components.hi, _MM_SHUFFLE(0, 0, 0, 0))}};
#endif
  }

  v_f32_8 yyyy(const v_f32_8 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return yyyy(v.components);
#else
    return {{_mm_shuffle_ps(v.components.lo, v.components.lo, _MM_SHUFFLE(1, 1, 1, 1)),
             _mm_shuffle_ps(v.components.hi, v.components.hi, _MM_SHUFFLE(1, 1, 1, 1))}};
#endif
  }

  v_f32_8 zzzz(const v_f32_8 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return zzzz(v.components);
#else
    return {{_mm_shuffle_ps(v.components.lo, v.components.lo, _MM_SHUFFLE(2, 2, 2, 2)),
             _mm_shuffle_ps(v.components.hi, v.components.hi, _MM_SHUFFLE(2, 2, 2, 2))}};
#endif
  }

  v_f32_8 wwww(const v_f32_8 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return wwww(v.components);
#else
    return {{_mm_shuffle_ps(v.components.lo, v.components.lo, _MM_SHUFFLE(3, 3, 3, 3)),
             _mm_shuffle_ps(v.components.hi, v.components.hi, _MM_SHUFFLE(3, 3, 3, 3))}};
#endif
  }

  v_f32_8 xxyy(const v_f32_8 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return xxyy(v.components);
#else
    return {{_mm_unpacklo_ps(v.components.lo, v.components.lo),
             _mm_unpacklo_ps(v.components.hi, v.components.hi)}};
#endif
  }

  v_f32_8 xyxy(const v_f32_8 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return xyxy(v.components);
#else
    return {{_mm_movelh_ps(v.components.lo, v.components.lo),
             _mm_movelh_ps(v.components.hi, v.components.hi)}};
#endif
  }

  v_f32_8 yyww(const v_f32_8 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return yyww(v.components);
#else
    return {{_mm_shuffle_ps(v.components.lo, v.components.lo, _MM_SHUFFLE(3, 3, 1, 1)),
             _mm_shuffle_ps(v.components.hi, v.components.hi, _MM_SHUFFLE(3, 3, 1, 1))}};
#endif
  }

  v_f32_8 zzww(const v_f32_8 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return zzww(v.components);
#else
    return {{_mm_unpackhi_ps(v.components.lo, v.components.lo),
             _mm_unpackhi_ps(v.components.hi, v.components.hi)}};
#endif
  }

  v_f32_8 zwzw(const v_f32_8 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return zwzw(v.components);
#else
    return {{_mm_movehl_ps(v.components.lo, v.components.lo),
             _mm_movehl_ps(v.components.hi, v.components.hi)}};
#endif
  }
}
