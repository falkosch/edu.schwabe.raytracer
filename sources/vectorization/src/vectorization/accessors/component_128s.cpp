#include "vectorization/accessors/component_128s.h"

#include "vectorization/swizzles.h"

namespace vectorization {
  template <>
  Float_32 component<VectorIndices::X>(const PackedFloat4_128 &values) noexcept {
    return _mm_cvtss_f32(values);
  }

  template <>
  Float_32 component<VectorIndices::Y>(const PackedFloat4_128 &values) noexcept {
    return component<VectorIndices::X>(yyww(values));
  }

  template <>
  Float_32 component<VectorIndices::Z>(const PackedFloat4_128 &values) noexcept {
    return component<VectorIndices::X>(zwzw(values));
  }

  template <>
  Float_32 component<VectorIndices::W>(const PackedFloat4_128 &values) noexcept {
    return component<VectorIndices::X>(wwww(values));
  }

  Float_32 x(const PackedFloat4_128 &values) noexcept {
    return component<VectorIndices::X>(values);
  }

  Float_32 y(const PackedFloat4_128 &values) noexcept {
    return component<VectorIndices::Y>(values);
  }

  Float_32 z(const PackedFloat4_128 &values) noexcept {
    return component<VectorIndices::Z>(values);
  }

  Float_32 w(const PackedFloat4_128 &values) noexcept {
    return component<VectorIndices::W>(values);
  }

  Float_32 x1(const PackedFloat4_128 &values) noexcept {
    return component<VectorIndices::X1>(values);
  }

  Float_32 x2(const PackedFloat4_128 &values) noexcept {
    return component<VectorIndices::X2>(values);
  }

  Float_32 x3(const PackedFloat4_128 &values) noexcept {
    return component<VectorIndices::X3>(values);
  }

  Float_32 x4(const PackedFloat4_128 &values) noexcept {
    return component<VectorIndices::X4>(values);
  }
}
