#include "vectorization/swizzledBlends/swizzledBlend2x2_128s.h"

namespace vectorization {
  template <>
  const PackedFloat4_128 swizzledBlend2x2<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(
      const PackedFloat4_128 &a, const PackedFloat4_128 &b
  ) noexcept {
    return _mm_movelh_ps(a, b);
  }

  template <>
  const PackedFloat4_128 swizzledBlend2x2<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(
      const PackedFloat4_128 &a, const PackedFloat4_128 &b
  ) noexcept {
    return _mm_movehl_ps(b, a);
  }

  const PackedFloat4_128 xx_xx(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept {
    return swizzledBlend2x2<VectorIndices::X, VectorIndices::X, VectorIndices::X, VectorIndices::X>(a, b);
  }

  const PackedFloat4_128 xx_yy(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept {
    return swizzledBlend2x2<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y>(a, b);
  }

  const PackedFloat4_128 xy_xy(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept {
    return swizzledBlend2x2<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(a, b);
  }

  const PackedFloat4_128 xy_zw(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept {
    return swizzledBlend2x2<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(a, b);
  }

  const PackedFloat4_128 xz_xz(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept {
    return swizzledBlend2x2<VectorIndices::X, VectorIndices::Z, VectorIndices::X, VectorIndices::Z>(a, b);
  }

  const PackedFloat4_128 yx_xy(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept {
    return swizzledBlend2x2<VectorIndices::Y, VectorIndices::X, VectorIndices::X, VectorIndices::Y>(a, b);
  }

  const PackedFloat4_128 yy_yy(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept {
    return swizzledBlend2x2<VectorIndices::Y, VectorIndices::Y, VectorIndices::Y, VectorIndices::Y>(a, b);
  }

  const PackedFloat4_128 yw_yw(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept {
    return swizzledBlend2x2<VectorIndices::Y, VectorIndices::W, VectorIndices::Y, VectorIndices::W>(a, b);
  }

  const PackedFloat4_128 zz_yy(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept {
    return swizzledBlend2x2<VectorIndices::Z, VectorIndices::Z, VectorIndices::Y, VectorIndices::Y>(a, b);
  }

  const PackedFloat4_128 zz_zz(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept {
    return swizzledBlend2x2<VectorIndices::Z, VectorIndices::Z, VectorIndices::Z, VectorIndices::Z>(a, b);
  }

  const PackedFloat4_128 zw_zw(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept {
    return swizzledBlend2x2<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(a, b);
  }

  const PackedFloat4_128 ww_ww(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept {
    return swizzledBlend2x2<VectorIndices::W, VectorIndices::W, VectorIndices::W, VectorIndices::W>(a, b);
  }
}