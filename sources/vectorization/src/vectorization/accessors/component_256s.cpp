#include "vectorization/accessors/component_256s.h"

#include "vectorization/swizzles.h"

namespace vectorization {
  inline PackedFloat8_256 hi128_lo128(const PackedFloat8_256 &values) {
    return swizzle<
        VectorIndices::X5, VectorIndices::X6, VectorIndices::X7, VectorIndices::X8, VectorIndices::X5,
        VectorIndices::X6, VectorIndices::X7, VectorIndices::X8>(values);
  }

  template <>
  Float_32 component<VectorIndices::X1>(const PackedFloat8_256 &values) noexcept {
    return _mm256_cvtss_f32(values);
  }

  template <>
  Float_32 component<VectorIndices::X2>(const PackedFloat8_256 &values) noexcept {
    return component<VectorIndices::X1>(yyww(values));
  }

  template <>
  Float_32 component<VectorIndices::X3>(const PackedFloat8_256 &values) noexcept {
    return component<VectorIndices::X1>(zzww(values));
  }

  template <>
  Float_32 component<VectorIndices::X4>(const PackedFloat8_256 &values) noexcept {
    return component<VectorIndices::X1>(wwww(values));
  }

  template <>
  Float_32 component<VectorIndices::X5>(const PackedFloat8_256 &values) noexcept {
    return component<VectorIndices::X1>(hi128_lo128(values));
  }

  template <>
  Float_32 component<VectorIndices::X6>(const PackedFloat8_256 &values) noexcept {
    return component<VectorIndices::X2>(hi128_lo128(values));
  }

  template <>
  Float_32 component<VectorIndices::X7>(const PackedFloat8_256 &values) noexcept {
    return component<VectorIndices::X3>(hi128_lo128(values));
  }

  template <>
  Float_32 component<VectorIndices::X8>(const PackedFloat8_256 &values) noexcept {
    return component<VectorIndices::X4>(hi128_lo128(values));
  }

  Float_32 x(const PackedFloat8_256 &values) noexcept {
    return component<VectorIndices::X>(values);
  }

  Float_32 y(const PackedFloat8_256 &values) noexcept {
    return component<VectorIndices::Y>(values);
  }

  Float_32 z(const PackedFloat8_256 &values) noexcept {
    return component<VectorIndices::Z>(values);
  }

  Float_32 w(const PackedFloat8_256 &values) noexcept {
    return component<VectorIndices::W>(values);
  }

  Float_32 x1(const PackedFloat8_256 &values) noexcept {
    return component<VectorIndices::X1>(values);
  }

  Float_32 x2(const PackedFloat8_256 &values) noexcept {
    return component<VectorIndices::X2>(values);
  }

  Float_32 x3(const PackedFloat8_256 &values) noexcept {
    return component<VectorIndices::X3>(values);
  }

  Float_32 x4(const PackedFloat8_256 &values) noexcept {
    return component<VectorIndices::X4>(values);
  }

  Float_32 x5(const PackedFloat8_256 &values) noexcept {
    return component<VectorIndices::X5>(values);
  }

  Float_32 x6(const PackedFloat8_256 &values) noexcept {
    return component<VectorIndices::X6>(values);
  }

  Float_32 x7(const PackedFloat8_256 &values) noexcept {
    return component<VectorIndices::X7>(values);
  }

  Float_32 x8(const PackedFloat8_256 &values) noexcept {
    return component<VectorIndices::X8>(values);
  }
}
