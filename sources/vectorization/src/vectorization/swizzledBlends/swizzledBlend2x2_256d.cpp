#include "vectorization/swizzledBlends/swizzledBlend2x2_256d.h"

namespace vectorization {
  const PackedFloat4_256 xx_xx(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept {
    return swizzledBlend2x2<VectorIndices::X, VectorIndices::X, VectorIndices::X, VectorIndices::X>(a, b);
  }

  const PackedFloat4_256 xx_yy(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept {
    return swizzledBlend2x2<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y>(a, b);
  }

  const PackedFloat4_256 xy_xy(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept {
    return swizzledBlend2x2<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(a, b);
  }

  const PackedFloat4_256 xy_zw(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept {
    return swizzledBlend2x2<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(a, b);
  }

  const PackedFloat4_256 xz_xz(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept {
    return swizzledBlend2x2<VectorIndices::X, VectorIndices::Z, VectorIndices::X, VectorIndices::Z>(a, b);
  }

  const PackedFloat4_256 yx_xy(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept {
    return swizzledBlend2x2<VectorIndices::Y, VectorIndices::X, VectorIndices::X, VectorIndices::Y>(a, b);
  }

  const PackedFloat4_256 yy_yy(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept {
    return swizzledBlend2x2<VectorIndices::Y, VectorIndices::Y, VectorIndices::Y, VectorIndices::Y>(a, b);
  }

  const PackedFloat4_256 yw_yw(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept {
    return swizzledBlend2x2<VectorIndices::Y, VectorIndices::W, VectorIndices::Y, VectorIndices::W>(a, b);
  }

  const PackedFloat4_256 zz_yy(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept {
    return swizzledBlend2x2<VectorIndices::Z, VectorIndices::Z, VectorIndices::Y, VectorIndices::Y>(a, b);
  }

  const PackedFloat4_256 zz_zz(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept {
    return swizzledBlend2x2<VectorIndices::Z, VectorIndices::Z, VectorIndices::Z, VectorIndices::Z>(a, b);
  }

  const PackedFloat4_256 zw_zw(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept {
    return swizzledBlend2x2<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(a, b);
  }

  const PackedFloat4_256 ww_ww(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept {
    return swizzledBlend2x2<VectorIndices::W, VectorIndices::W, VectorIndices::W, VectorIndices::W>(a, b);
  }
}