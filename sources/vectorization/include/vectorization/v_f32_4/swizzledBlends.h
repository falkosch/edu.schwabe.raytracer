#pragma once

#include "type.h"
#include "vectorization/swizzledBlends.h"

namespace vectorization {
  // generic swizzled blend
  template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W, bool SelectX, bool SelectY, bool SelectZ, bool SelectW>
  inline const v_f32_4 swizzledBlend(const v_f32_4 &a, const v_f32_4 &b) noexcept {
    return swizzledBlend<X, Y, Z, W, SelectX, SelectY, SelectZ, SelectW>(a.components, b.components);
  }

  // Special case swizzled blend of two vectors, in which only the first two
  // components of this vector and the last two components of the other
  // vector are taken for blending after swizzling.
  template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W>
  inline const v_f32_4 swizzledBlend2x2(const v_f32_4 &a, const v_f32_4 &b) noexcept {
    static_assert(X < v_f32_4::SIZE, "Index is out of range");
    static_assert(Y < v_f32_4::SIZE, "Index is out of range");
    static_assert(Z < v_f32_4::SIZE, "Index is out of range");
    static_assert(W < v_f32_4::SIZE, "Index is out of range");
    return swizzledBlend2x2<X, Y, Z, W>(a.components, b.components);
  }

  const v_f32_4 xx_xx(const v_f32_4 &a, const v_f32_4 &b) noexcept;

  const v_f32_4 xx_yy(const v_f32_4 &a, const v_f32_4 &b) noexcept;

  const v_f32_4 xy_xy(const v_f32_4 &a, const v_f32_4 &b) noexcept;

  const v_f32_4 xy_zw(const v_f32_4 &a, const v_f32_4 &b) noexcept;

  const v_f32_4 xz_xz(const v_f32_4 &a, const v_f32_4 &b) noexcept;

  const v_f32_4 yx_xy(const v_f32_4 &a, const v_f32_4 &b) noexcept;

  const v_f32_4 yy_yy(const v_f32_4 &a, const v_f32_4 &b) noexcept;

  const v_f32_4 yw_yw(const v_f32_4 &a, const v_f32_4 &b) noexcept;

  const v_f32_4 zz_yy(const v_f32_4 &a, const v_f32_4 &b) noexcept;

  const v_f32_4 zz_zz(const v_f32_4 &a, const v_f32_4 &b) noexcept;

  const v_f32_4 zw_zw(const v_f32_4 &a, const v_f32_4 &b) noexcept;

  const v_f32_4 ww_ww(const v_f32_4 &a, const v_f32_4 &b) noexcept;

  template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W>
  inline const v_f32_4 swizzledBlendMasked(const v_f32_4 &a, const v_f32_4 &b, const v_f32_4 &mask) noexcept {
    return swizzledBlendMasked<X, Y, Z, W>(a.components, b.components, mask.components);
  }

  template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W>
  inline const v_f32_4
  swizzledBlendMasked(const v_f32_4 &a, const v_f32_4 &b, const v_f32_4::VectorBoolType &mask) noexcept {
    return swizzledBlendMasked<X, Y, Z, W>(a.components, b.components, mask.components);
  }
}
