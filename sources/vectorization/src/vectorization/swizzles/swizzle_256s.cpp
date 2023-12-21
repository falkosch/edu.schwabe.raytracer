#include "vectorization/swizzles/swizzle_256s.h"

namespace vectorization {
  template <>
  const PackedFloat8_256
  swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(const PackedFloat8_256 &v) noexcept {
    return v;
  }

  template <>
  const PackedFloat8_256
  swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y>(const PackedFloat8_256 &v) noexcept {
    return _mm256_unpacklo_ps(v, v);
  }

  template <>
  const PackedFloat8_256
  swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Z, VectorIndices::Z>(const PackedFloat8_256 &v) noexcept {
    return _mm256_moveldup_ps(v);
  }

  template <>
  const PackedFloat8_256
  swizzle<VectorIndices::Y, VectorIndices::Y, VectorIndices::W, VectorIndices::W>(const PackedFloat8_256 &v) noexcept {
    return _mm256_movehdup_ps(v);
  }

  template <>
  const PackedFloat8_256
  swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(const PackedFloat8_256 &v) noexcept {
    return _mm256_unpackhi_ps(v, v);
  }

  const PackedFloat8_256 xxxx(const PackedFloat8_256 &v) noexcept {
    return swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::X, VectorIndices::X>(v);
  }

  const PackedFloat8_256 xxxz(const PackedFloat8_256 &v) noexcept {
    return swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::X, VectorIndices::Z>(v);
  }

  const PackedFloat8_256 xxyy(const PackedFloat8_256 &v) noexcept {
    return swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y>(v);
  }

  const PackedFloat8_256 xxzz(const PackedFloat8_256 &v) noexcept {
    return swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Z, VectorIndices::Z>(v);
  }

  const PackedFloat8_256 xyxy(const PackedFloat8_256 &v) noexcept {
    return swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(v);
  }

  const PackedFloat8_256 xzzz(const PackedFloat8_256 &v) noexcept {
    return swizzle<VectorIndices::X, VectorIndices::Z, VectorIndices::Z, VectorIndices::Z>(v);
  }

  const PackedFloat8_256 yyyy(const PackedFloat8_256 &v) noexcept {
    return swizzle<VectorIndices::Y, VectorIndices::Y, VectorIndices::Y, VectorIndices::Y>(v);
  }

  const PackedFloat8_256 yxxy(const PackedFloat8_256 &v) noexcept {
    return swizzle<VectorIndices::Y, VectorIndices::X, VectorIndices::X, VectorIndices::Y>(v);
  }

  const PackedFloat8_256 yxwz(const PackedFloat8_256 &v) noexcept {
    return swizzle<VectorIndices::Y, VectorIndices::X, VectorIndices::W, VectorIndices::Z>(v);
  }

  const PackedFloat8_256 yyww(const PackedFloat8_256 &v) noexcept {
    return swizzle<VectorIndices::Y, VectorIndices::Y, VectorIndices::W, VectorIndices::W>(v);
  }

  const PackedFloat8_256 yzxw(const PackedFloat8_256 &v) noexcept {
    return swizzle<VectorIndices::Y, VectorIndices::Z, VectorIndices::X, VectorIndices::W>(v);
  }

  const PackedFloat8_256 yzwx(const PackedFloat8_256 &v) noexcept {
    return swizzle<VectorIndices::Y, VectorIndices::Z, VectorIndices::W, VectorIndices::X>(v);
  }

  const PackedFloat8_256 zzzz(const PackedFloat8_256 &v) noexcept {
    return swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::Z, VectorIndices::Z>(v);
  }

  const PackedFloat8_256 zxyw(const PackedFloat8_256 &v) noexcept {
    return swizzle<VectorIndices::Z, VectorIndices::X, VectorIndices::Y, VectorIndices::W>(v);
  }

  const PackedFloat8_256 zzyy(const PackedFloat8_256 &v) noexcept {
    return swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::Y, VectorIndices::Y>(v);
  }

  const PackedFloat8_256 zzww(const PackedFloat8_256 &v) noexcept {
    return swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(v);
  }

  const PackedFloat8_256 zwxy(const PackedFloat8_256 &v) noexcept {
    return swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::X, VectorIndices::Y>(v);
  }

  const PackedFloat8_256 zwzw(const PackedFloat8_256 &v) noexcept {
    return swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(v);
  }

  const PackedFloat8_256 wwww(const PackedFloat8_256 &v) noexcept {
    return swizzle<VectorIndices::W, VectorIndices::W, VectorIndices::W, VectorIndices::W>(v);
  }

  const PackedFloat8_256 wxyz(const PackedFloat8_256 &v) noexcept {
    return swizzle<VectorIndices::W, VectorIndices::X, VectorIndices::Y, VectorIndices::Z>(v);
  }

  const PackedFloat8_256 wzyx(const PackedFloat8_256 &v) noexcept {
    return swizzle<VectorIndices::W, VectorIndices::Z, VectorIndices::Y, VectorIndices::X>(v);
  }

  template <>
  const PackedFloat8_256 swizzle<
      VectorIndices::X1, VectorIndices::X2, VectorIndices::X3, VectorIndices::X4, VectorIndices::X5, VectorIndices::X6,
      VectorIndices::X7, VectorIndices::X8>(const PackedFloat8_256 &v) noexcept {
    return v;
  }

  template <>
  const PackedFloat8_256 swizzle<
      VectorIndices::X1, VectorIndices::X1, VectorIndices::X2, VectorIndices::X2, VectorIndices::X5, VectorIndices::X5,
      VectorIndices::X6, VectorIndices::X6>(const PackedFloat8_256 &v) noexcept {
    return _mm256_unpacklo_ps(v, v);
  }

  template <>
  const PackedFloat8_256 swizzle<
      VectorIndices::X1, VectorIndices::X1, VectorIndices::X3, VectorIndices::X3, VectorIndices::X5, VectorIndices::X5,
      VectorIndices::X7, VectorIndices::X7>(const PackedFloat8_256 &v) noexcept {
    return _mm256_moveldup_ps(v);
  }

  template <>
  const PackedFloat8_256 swizzle<
      VectorIndices::X1, VectorIndices::X2, VectorIndices::X3, VectorIndices::X4, VectorIndices::X1, VectorIndices::X2,
      VectorIndices::X3, VectorIndices::X4>(const PackedFloat8_256 &v) noexcept {
    return _mm256_permute2f128_ps(v, v, 0b00000000);
  }

  template <>
  const PackedFloat8_256 swizzle<
      VectorIndices::X2, VectorIndices::X2, VectorIndices::X4, VectorIndices::X4, VectorIndices::X6, VectorIndices::X6,
      VectorIndices::X8, VectorIndices::X8>(const PackedFloat8_256 &v) noexcept {
    return _mm256_movehdup_ps(v);
  }

  template <>
  const PackedFloat8_256 swizzle<
      VectorIndices::X3, VectorIndices::X3, VectorIndices::X4, VectorIndices::X4, VectorIndices::X7, VectorIndices::X7,
      VectorIndices::X8, VectorIndices::X8>(const PackedFloat8_256 &v) noexcept {
    return _mm256_unpackhi_ps(v, v);
  }

  template <>
  const PackedFloat8_256 swizzle<
      VectorIndices::X5, VectorIndices::X6, VectorIndices::X7, VectorIndices::X8, VectorIndices::X1, VectorIndices::X2,
      VectorIndices::X3, VectorIndices::X4>(const PackedFloat8_256 &v) noexcept {
    return _mm256_permute2f128_ps(v, v, 0b00000001);
  }

  template <>
  const PackedFloat8_256 swizzle<
      VectorIndices::X5, VectorIndices::X6, VectorIndices::X7, VectorIndices::X8, VectorIndices::X5, VectorIndices::X6,
      VectorIndices::X7, VectorIndices::X8>(const PackedFloat8_256 &v) noexcept {
    return _mm256_permute2f128_ps(v, v, 0b00010001);
  }
}