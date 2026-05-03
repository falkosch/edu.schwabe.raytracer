#include "vectorization/v_f64_4/swizzles.h"

#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
#include "vectorization/swizzles/swizzle_256d.h"
#endif

namespace vectorization {
  template <>
  v_f64_4 swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(const v_f64_4 &v) noexcept {
    return v;
  }

  v_f64_4 xxxx(const v_f64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return xxxx(v.components);
#else
    return v_f64_4(v[VectorIndices::X], v[VectorIndices::X], v[VectorIndices::X], v[VectorIndices::X]);
#endif
  }

  v_f64_4 yyyy(const v_f64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return yyyy(v.components);
#else
    return v_f64_4(v[VectorIndices::Y], v[VectorIndices::Y], v[VectorIndices::Y], v[VectorIndices::Y]);
#endif
  }

  v_f64_4 zzzz(const v_f64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return zzzz(v.components);
#else
    return v_f64_4(v[VectorIndices::Z], v[VectorIndices::Z], v[VectorIndices::Z], v[VectorIndices::Z]);
#endif
  }

  v_f64_4 wwww(const v_f64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return wwww(v.components);
#else
    return v_f64_4(v[VectorIndices::W], v[VectorIndices::W], v[VectorIndices::W], v[VectorIndices::W]);
#endif
  }

  v_f64_4 xxxz(const v_f64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return xxxz(v.components);
#else
    return v_f64_4(v[VectorIndices::X], v[VectorIndices::X], v[VectorIndices::X], v[VectorIndices::Z]);
#endif
  }

  v_f64_4 xxyy(const v_f64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return xxyy(v.components);
#else
    return v_f64_4(v[VectorIndices::X], v[VectorIndices::X], v[VectorIndices::Y], v[VectorIndices::Y]);
#endif
  }

  v_f64_4 xxzz(const v_f64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return xxzz(v.components);
#else
    return v_f64_4(v[VectorIndices::X], v[VectorIndices::X], v[VectorIndices::Z], v[VectorIndices::Z]);
#endif
  }

  v_f64_4 xyxy(const v_f64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return xyxy(v.components);
#else
    return v_f64_4(v[VectorIndices::X], v[VectorIndices::Y], v[VectorIndices::X], v[VectorIndices::Y]);
#endif
  }

  v_f64_4 xzzz(const v_f64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return xzzz(v.components);
#else
    return v_f64_4(v[VectorIndices::X], v[VectorIndices::Z], v[VectorIndices::Z], v[VectorIndices::Z]);
#endif
  }

  v_f64_4 yxxy(const v_f64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return yxxy(v.components);
#else
    return v_f64_4(v[VectorIndices::Y], v[VectorIndices::X], v[VectorIndices::X], v[VectorIndices::Y]);
#endif
  }

  v_f64_4 yxwz(const v_f64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return yxwz(v.components);
#else
    return v_f64_4(v[VectorIndices::Y], v[VectorIndices::X], v[VectorIndices::W], v[VectorIndices::Z]);
#endif
  }

  v_f64_4 yyww(const v_f64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return yyww(v.components);
#else
    return v_f64_4(v[VectorIndices::Y], v[VectorIndices::Y], v[VectorIndices::W], v[VectorIndices::W]);
#endif
  }

  v_f64_4 yzxw(const v_f64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return yzxw(v.components);
#else
    return v_f64_4(v[VectorIndices::Y], v[VectorIndices::Z], v[VectorIndices::X], v[VectorIndices::W]);
#endif
  }

  v_f64_4 yzwx(const v_f64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return yzwx(v.components);
#else
    return v_f64_4(v[VectorIndices::Y], v[VectorIndices::Z], v[VectorIndices::W], v[VectorIndices::X]);
#endif
  }

  v_f64_4 zxyw(const v_f64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return zxyw(v.components);
#else
    return v_f64_4(v[VectorIndices::Z], v[VectorIndices::X], v[VectorIndices::Y], v[VectorIndices::W]);
#endif
  }

  v_f64_4 zzyy(const v_f64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return zzyy(v.components);
#else
    return v_f64_4(v[VectorIndices::Z], v[VectorIndices::Z], v[VectorIndices::Y], v[VectorIndices::Y]);
#endif
  }

  v_f64_4 zzww(const v_f64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return zzww(v.components);
#else
    return v_f64_4(v[VectorIndices::Z], v[VectorIndices::Z], v[VectorIndices::W], v[VectorIndices::W]);
#endif
  }

  v_f64_4 zwxy(const v_f64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return zwxy(v.components);
#else
    return v_f64_4(v[VectorIndices::Z], v[VectorIndices::W], v[VectorIndices::X], v[VectorIndices::Y]);
#endif
  }

  v_f64_4 zwzw(const v_f64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return zwzw(v.components);
#else
    return v_f64_4(v[VectorIndices::Z], v[VectorIndices::W], v[VectorIndices::Z], v[VectorIndices::W]);
#endif
  }

  v_f64_4 wxyz(const v_f64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return wxyz(v.components);
#else
    return v_f64_4(v[VectorIndices::W], v[VectorIndices::X], v[VectorIndices::Y], v[VectorIndices::Z]);
#endif
  }

  v_f64_4 wzyx(const v_f64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return wzyx(v.components);
#else
    return v_f64_4(v[VectorIndices::W], v[VectorIndices::Z], v[VectorIndices::Y], v[VectorIndices::X]);
#endif
  }
}
