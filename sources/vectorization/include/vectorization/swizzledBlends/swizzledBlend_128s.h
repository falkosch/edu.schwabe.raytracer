#pragma once

#include "vectorization/architecture.h"
#include "vectorization/blends.h"
#include "vectorization/swizzles.h"

namespace vectorization {
  // generic swizzled blend
  template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W, bool SelectX, bool SelectY, bool SelectZ, bool SelectW>
  inline PackedFloat4_128 swizzledBlend(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept {
    return blend<SelectX, SelectY, SelectZ, SelectW>(swizzle<X, Y, Z, W>(a), swizzle<X, Y, Z, W>(b));
  }

  // x(a), x(b), y(a), y(b)
  template <>
  PackedFloat4_128
  swizzledBlend<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y, false, true, false, true>(
      const PackedFloat4_128 &a, const PackedFloat4_128 &b
  ) noexcept;

  // x(b), x(a), y(b), y(a)
  template <>
  PackedFloat4_128
  swizzledBlend<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y, true, false, true, false>(
      const PackedFloat4_128 &a, const PackedFloat4_128 &b
  ) noexcept;

  // x(a), y(a), x(b), y(b)
  template <>
  PackedFloat4_128
  swizzledBlend<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y, false, false, true, true>(
      const PackedFloat4_128 &a, const PackedFloat4_128 &b
  ) noexcept;

  // x(b), y(b), x(a), y(a)
  template <>
  PackedFloat4_128
  swizzledBlend<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y, true, true, false, false>(
      const PackedFloat4_128 &a, const PackedFloat4_128 &b
  ) noexcept;

  // z(a), z(b), w(a), w(b)
  template <>
  PackedFloat4_128
  swizzledBlend<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W, false, true, false, true>(
      const PackedFloat4_128 &a, const PackedFloat4_128 &b
  ) noexcept;

  // z(b), z(a), w(b), w(a)
  template <>
  PackedFloat4_128
  swizzledBlend<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W, true, false, true, false>(
      const PackedFloat4_128 &a, const PackedFloat4_128 &b
  ) noexcept;

  // z(b), w(b), z(a), w(a)
  template <>
  PackedFloat4_128
  swizzledBlend<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W, true, true, false, false>(
      const PackedFloat4_128 &a, const PackedFloat4_128 &b
  ) noexcept;

  // z(a), w(a), z(b), w(b)
  template <>
  PackedFloat4_128
  swizzledBlend<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W, false, false, true, true>(
      const PackedFloat4_128 &a, const PackedFloat4_128 &b
  ) noexcept;
}
