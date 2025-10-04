#include "vectorization/swizzles/swizzle_256d.h"

namespace vectorization {
  template <>
  PackedFloat4_256
  swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(const PackedFloat4_256 &v) noexcept {
    return v;
  }

  template <>
  PackedFloat4_256
  swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Z, VectorIndices::Z>(const PackedFloat4_256 &v) noexcept {
    return _mm256_movedup_pd(v);
  }

  template <>
  PackedFloat4_256
  swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(const PackedFloat4_256 &v) noexcept {
    return _mm256_permute2f128_pd(v, v, 0b00000000);
  }

  template <>
  PackedFloat4_256
  swizzle<VectorIndices::Y, VectorIndices::Y, VectorIndices::W, VectorIndices::W>(const PackedFloat4_256 &v) noexcept {
    return _mm256_unpackhi_pd(v, v);
  }

  template <>
  PackedFloat4_256
  swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::X, VectorIndices::Y>(const PackedFloat4_256 &v) noexcept {
    return _mm256_permute2f128_pd(v, v, 0b00000001);
  }

  template <>
  PackedFloat4_256
  swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(const PackedFloat4_256 &v) noexcept {
    return _mm256_permute2f128_pd(v, v, 0b00010001);
  }

  PackedFloat4_256 xxxx(const PackedFloat4_256 &values) noexcept {
    return swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::X, VectorIndices::X>(values);
  }

  PackedFloat4_256 xxxz(const PackedFloat4_256 &values) noexcept {
    return swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::X, VectorIndices::Z>(values);
  }

  PackedFloat4_256 xxyy(const PackedFloat4_256 &values) noexcept {
    return swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y>(values);
  }

  PackedFloat4_256 xxzz(const PackedFloat4_256 &values) noexcept {
    return swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Z, VectorIndices::Z>(values);
  }

  PackedFloat4_256 xyxy(const PackedFloat4_256 &values) noexcept {
    return swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(values);
  }

  PackedFloat4_256 xzzz(const PackedFloat4_256 &values) noexcept {
    return swizzle<VectorIndices::X, VectorIndices::Z, VectorIndices::Z, VectorIndices::Z>(values);
  }

  PackedFloat4_256 yyyy(const PackedFloat4_256 &values) noexcept {
    return swizzle<VectorIndices::Y, VectorIndices::Y, VectorIndices::Y, VectorIndices::Y>(values);
  }

  PackedFloat4_256 yxxy(const PackedFloat4_256 &values) noexcept {
    return swizzle<VectorIndices::Y, VectorIndices::X, VectorIndices::X, VectorIndices::Y>(values);
  }

  PackedFloat4_256 yxwz(const PackedFloat4_256 &values) noexcept {
    return swizzle<VectorIndices::Y, VectorIndices::X, VectorIndices::W, VectorIndices::Z>(values);
  }

  PackedFloat4_256 yyww(const PackedFloat4_256 &values) noexcept {
    return swizzle<VectorIndices::Y, VectorIndices::Y, VectorIndices::W, VectorIndices::W>(values);
  }

  PackedFloat4_256 yzxw(const PackedFloat4_256 &values) noexcept {
    return swizzle<VectorIndices::Y, VectorIndices::Z, VectorIndices::X, VectorIndices::W>(values);
  }

  PackedFloat4_256 yzwx(const PackedFloat4_256 &values) noexcept {
    return swizzle<VectorIndices::Y, VectorIndices::Z, VectorIndices::W, VectorIndices::X>(values);
  }

  PackedFloat4_256 zzzz(const PackedFloat4_256 &values) noexcept {
    return swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::Z, VectorIndices::Z>(values);
  }

  PackedFloat4_256 zxyw(const PackedFloat4_256 &values) noexcept {
    return swizzle<VectorIndices::Z, VectorIndices::X, VectorIndices::Y, VectorIndices::W>(values);
  }

  PackedFloat4_256 zzyy(const PackedFloat4_256 &values) noexcept {
    return swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::Y, VectorIndices::Y>(values);
  }

  PackedFloat4_256 zzww(const PackedFloat4_256 &values) noexcept {
    return swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(values);
  }

  PackedFloat4_256 zwxy(const PackedFloat4_256 &values) noexcept {
    return swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::X, VectorIndices::Y>(values);
  }

  PackedFloat4_256 zwzw(const PackedFloat4_256 &values) noexcept {
    return swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(values);
  }

  PackedFloat4_256 wwww(const PackedFloat4_256 &values) noexcept {
    return swizzle<VectorIndices::W, VectorIndices::W, VectorIndices::W, VectorIndices::W>(values);
  }

  PackedFloat4_256 wxyz(const PackedFloat4_256 &values) noexcept {
    return swizzle<VectorIndices::W, VectorIndices::X, VectorIndices::Y, VectorIndices::Z>(values);
  }

  PackedFloat4_256 wzyx(const PackedFloat4_256 &values) noexcept {
    return swizzle<VectorIndices::W, VectorIndices::Z, VectorIndices::Y, VectorIndices::X>(values);
  }
}
