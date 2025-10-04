#include "vectorization/accessors/component_256d.h"

#include "vectorization/swizzles.h"

namespace vectorization {
  template <>
  const Float_64 component<VectorIndices::X>(const PackedFloat4_256 &v) noexcept {
    return _mm256_cvtsd_f64(v);
  }

  template <>
  const Float_64 component<VectorIndices::Y>(const PackedFloat4_256 &v) noexcept {
    return component<VectorIndices::X>(yyww(v));
  }

  template <>
  const Float_64 component<VectorIndices::Z>(const PackedFloat4_256 &v) noexcept {
    return component<VectorIndices::X>(zwzw(v));
  }

  template <>
  const Float_64 component<VectorIndices::W>(const PackedFloat4_256 &v) noexcept {
    return component<VectorIndices::Y>(zwzw(v));
  }

  const Float_64 x(const PackedFloat4_256 &v) noexcept {
    return component<VectorIndices::X>(v);
  }

  const Float_64 y(const PackedFloat4_256 &v) noexcept {
    return component<VectorIndices::Y>(v);
  }

  const Float_64 z(const PackedFloat4_256 &v) noexcept {
    return component<VectorIndices::Z>(v);
  }

  const Float_64 w(const PackedFloat4_256 &v) noexcept {
    return component<VectorIndices::W>(v);
  }

  const Float_64 x1(const PackedFloat4_256 &v) noexcept {
    return component<VectorIndices::X1>(v);
  }

  const Float_64 x2(const PackedFloat4_256 &v) noexcept {
    return component<VectorIndices::X2>(v);
  }

  const Float_64 x3(const PackedFloat4_256 &v) noexcept {
    return component<VectorIndices::X3>(v);
  }

  const Float_64 x4(const PackedFloat4_256 &v) noexcept {
    return component<VectorIndices::X4>(v);
  }
}