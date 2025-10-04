#include "vectorization/swizzles/swizzle_128s.h"

namespace vectorization {
  template <>
  PackedFloat4_128
  swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(const PackedFloat4_128 &v) noexcept {
    return v;
  }

  template <>
  PackedFloat4_128
  swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y>(const PackedFloat4_128 &v) noexcept {
    return _mm_unpacklo_ps(v, v);
  }

  template <>
  PackedFloat4_128
  swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Z, VectorIndices::Z>(const PackedFloat4_128 &v) noexcept {
    return _mm_moveldup_ps(v);
  }

  template <>
  PackedFloat4_128
  swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(const PackedFloat4_128 &v) noexcept {
    return _mm_movelh_ps(v, v);
  }

  template <>
  PackedFloat4_128
  swizzle<VectorIndices::Y, VectorIndices::Y, VectorIndices::W, VectorIndices::W>(const PackedFloat4_128 &v) noexcept {
    return _mm_movehdup_ps(v);
  }

  template <>
  PackedFloat4_128
  swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(const PackedFloat4_128 &v) noexcept {
    return _mm_unpackhi_ps(v, v);
  }

  template <>
  PackedFloat4_128
  swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(const PackedFloat4_128 &v) noexcept {
    return _mm_movehl_ps(v, v);
  }

  PackedFloat4_128 xxxx(const PackedFloat4_128 &values) noexcept {
    return swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::X, VectorIndices::X>(values);
  }

  PackedFloat4_128 yyyy(const PackedFloat4_128 &values) noexcept {
    return swizzle<VectorIndices::Y, VectorIndices::Y, VectorIndices::Y, VectorIndices::Y>(values);
  }

  PackedFloat4_128 zzzz(const PackedFloat4_128 &values) noexcept {
    return swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::Z, VectorIndices::Z>(values);
  }

  PackedFloat4_128 wwww(const PackedFloat4_128 &values) noexcept {
    return swizzle<VectorIndices::W, VectorIndices::W, VectorIndices::W, VectorIndices::W>(values);
  }

  PackedFloat4_128 xxxz(const PackedFloat4_128 &values) noexcept {
    return swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::X, VectorIndices::Z>(values);
  }

  PackedFloat4_128 xxyy(const PackedFloat4_128 &values) noexcept {
    return swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y>(values);
  }

  PackedFloat4_128 xxzz(const PackedFloat4_128 &values) noexcept {
    return swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Z, VectorIndices::Z>(values);
  }

  PackedFloat4_128 xyxy(const PackedFloat4_128 &values) noexcept {
    return swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(values);
  }

  PackedFloat4_128 xzzz(const PackedFloat4_128 &values) noexcept {
    return swizzle<VectorIndices::X, VectorIndices::Z, VectorIndices::Z, VectorIndices::Z>(values);
  }

  PackedFloat4_128 yxxy(const PackedFloat4_128 &values) noexcept {
    return swizzle<VectorIndices::Y, VectorIndices::X, VectorIndices::X, VectorIndices::Y>(values);
  }

  PackedFloat4_128 yxwz(const PackedFloat4_128 &values) noexcept {
    return swizzle<VectorIndices::Y, VectorIndices::X, VectorIndices::W, VectorIndices::Z>(values);
  }

  PackedFloat4_128 yyww(const PackedFloat4_128 &values) noexcept {
    return swizzle<VectorIndices::Y, VectorIndices::Y, VectorIndices::W, VectorIndices::W>(values);
  }

  PackedFloat4_128 yzxw(const PackedFloat4_128 &values) noexcept {
    return swizzle<VectorIndices::Y, VectorIndices::Z, VectorIndices::X, VectorIndices::W>(values);
  }

  PackedFloat4_128 yzwx(const PackedFloat4_128 &values) noexcept {
    return swizzle<VectorIndices::Y, VectorIndices::Z, VectorIndices::W, VectorIndices::X>(values);
  }

  PackedFloat4_128 zxyw(const PackedFloat4_128 &values) noexcept {
    return swizzle<VectorIndices::Z, VectorIndices::X, VectorIndices::Y, VectorIndices::W>(values);
  }

  PackedFloat4_128 zzyy(const PackedFloat4_128 &values) noexcept {
    return swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::Y, VectorIndices::Y>(values);
  }

  PackedFloat4_128 zzww(const PackedFloat4_128 &values) noexcept {
    return swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(values);
  }

  PackedFloat4_128 zwxy(const PackedFloat4_128 &values) noexcept {
    return swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::X, VectorIndices::Y>(values);
  }

  PackedFloat4_128 zwzw(const PackedFloat4_128 &values) noexcept {
    return swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(values);
  }

  PackedFloat4_128 wxyz(const PackedFloat4_128 &values) noexcept {
    return swizzle<VectorIndices::W, VectorIndices::X, VectorIndices::Y, VectorIndices::Z>(values);
  }

  PackedFloat4_128 wzyx(const PackedFloat4_128 &values) noexcept {
    return swizzle<VectorIndices::W, VectorIndices::Z, VectorIndices::Y, VectorIndices::X>(values);
  }
}
