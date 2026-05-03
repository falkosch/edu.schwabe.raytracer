#pragma once

#include "type.h"
#include "vectorization/swizzles.h"

namespace vectorization {
  template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W>
  v_f64_4 swizzle(const v_f64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return swizzle<X, Y, Z, W>(v.components);
#else
    return v_f64_4(v[X], v[Y], v[Z], v[W]);
#endif
  }

  template <>
  v_f64_4 swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(const v_f64_4 &v) noexcept;

  v_f64_4 xxxx(const v_f64_4 &v) noexcept;
  v_f64_4 yyyy(const v_f64_4 &v) noexcept;
  v_f64_4 zzzz(const v_f64_4 &v) noexcept;
  v_f64_4 wwww(const v_f64_4 &v) noexcept;
  v_f64_4 xxxz(const v_f64_4 &v) noexcept;
  v_f64_4 xxyy(const v_f64_4 &v) noexcept;
  v_f64_4 xxzz(const v_f64_4 &v) noexcept;
  v_f64_4 xyxy(const v_f64_4 &v) noexcept;
  v_f64_4 xzzz(const v_f64_4 &v) noexcept;
  v_f64_4 yxxy(const v_f64_4 &v) noexcept;
  v_f64_4 yxwz(const v_f64_4 &v) noexcept;
  v_f64_4 yyww(const v_f64_4 &v) noexcept;
  v_f64_4 yzxw(const v_f64_4 &v) noexcept;
  v_f64_4 yzwx(const v_f64_4 &v) noexcept;
  v_f64_4 zxyw(const v_f64_4 &v) noexcept;
  v_f64_4 zzyy(const v_f64_4 &v) noexcept;
  v_f64_4 zzww(const v_f64_4 &v) noexcept;
  v_f64_4 zwxy(const v_f64_4 &v) noexcept;
  v_f64_4 zwzw(const v_f64_4 &v) noexcept;
  v_f64_4 wxyz(const v_f64_4 &v) noexcept;
  v_f64_4 wzyx(const v_f64_4 &v) noexcept;
}
