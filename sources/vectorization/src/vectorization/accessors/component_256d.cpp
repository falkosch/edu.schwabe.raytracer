#include "vectorization/accessors/component_256d.h"

#include "vectorization/swizzles.h"

namespace vectorization {
  template <>
  Float_64 component<VectorIndices::X>(const PackedFloat4_256 &values) noexcept {
    return _mm256_cvtsd_f64(values);
  }

  template <>
  Float_64 component<VectorIndices::Y>(const PackedFloat4_256 &values) noexcept {
    return component<VectorIndices::X>(yyww(values));
  }

  template <>
  Float_64 component<VectorIndices::Z>(const PackedFloat4_256 &values) noexcept {
    return component<VectorIndices::X>(zwzw(values));
  }

  template <>
  Float_64 component<VectorIndices::W>(const PackedFloat4_256 &values) noexcept {
    return component<VectorIndices::Y>(zwzw(values));
  }

  Float_64 x(const PackedFloat4_256 &values) noexcept {
    return component<VectorIndices::X>(values);
  }

  Float_64 y(const PackedFloat4_256 &values) noexcept {
    return component<VectorIndices::Y>(values);
  }

  Float_64 z(const PackedFloat4_256 &values) noexcept {
    return component<VectorIndices::Z>(values);
  }

  Float_64 w(const PackedFloat4_256 &values) noexcept {
    return component<VectorIndices::W>(values);
  }

  Float_64 x1(const PackedFloat4_256 &values) noexcept {
    return component<VectorIndices::X1>(values);
  }

  Float_64 x2(const PackedFloat4_256 &values) noexcept {
    return component<VectorIndices::X2>(values);
  }

  Float_64 x3(const PackedFloat4_256 &values) noexcept {
    return component<VectorIndices::X3>(values);
  }

  Float_64 x4(const PackedFloat4_256 &values) noexcept {
    return component<VectorIndices::X4>(values);
  }
}
