#include "vectorization/accessors/component_256i.h"

#include "vectorization/swizzles/swizzle_256i.h"

namespace vectorization {
  inline PackedInts_256 hi128_lo128(const PackedInts_256 &values) {
    return _mm256_permute2x128_si256(values, values, 0b00000001);
  }

  template <>
  Int_32 component<VectorIndices::X1>(const PackedInts_256 &values) noexcept {
    return _mm256_cvtsi256_si32(values);
  }

  template <>
  Int_32 component<VectorIndices::X2>(const PackedInts_256 &values) noexcept {
    return component<VectorIndices::X1>(yyww(values));
  }

  template <>
  Int_32 component<VectorIndices::X3>(const PackedInts_256 &values) noexcept {
    return component<VectorIndices::X1>(zzww(values));
  }

  template <>
  Int_32 component<VectorIndices::X4>(const PackedInts_256 &values) noexcept {
    return component<VectorIndices::X1>(wwww(values));
  }

  template <>
  Int_32 component<VectorIndices::X5>(const PackedInts_256 &values) noexcept {
    return component<VectorIndices::X1>(hi128_lo128(values));
  }

  template <>
  Int_32 component<VectorIndices::X6>(const PackedInts_256 &values) noexcept {
    return component<VectorIndices::X2>(hi128_lo128(values));
  }

  template <>
  Int_32 component<VectorIndices::X7>(const PackedInts_256 &values) noexcept {
    return component<VectorIndices::X3>(hi128_lo128(values));
  }

  template <>
  Int_32 component<VectorIndices::X8>(const PackedInts_256 &values) noexcept {
    return component<VectorIndices::X4>(hi128_lo128(values));
  }

  Int_32 x1(const PackedInts_256 &values) noexcept {
    return component<VectorIndices::X1>(values);
  }

  Int_32 x2(const PackedInts_256 &values) noexcept {
    return component<VectorIndices::X2>(values);
  }

  Int_32 x3(const PackedInts_256 &values) noexcept {
    return component<VectorIndices::X3>(values);
  }

  Int_32 x4(const PackedInts_256 &values) noexcept {
    return component<VectorIndices::X4>(values);
  }

  Int_32 x5(const PackedInts_256 &values) noexcept {
    return component<VectorIndices::X5>(values);
  }

  Int_32 x6(const PackedInts_256 &values) noexcept {
    return component<VectorIndices::X6>(values);
  }

  Int_32 x7(const PackedInts_256 &values) noexcept {
    return component<VectorIndices::X7>(values);
  }

  Int_32 x8(const PackedInts_256 &values) noexcept {
    return component<VectorIndices::X8>(values);
  }
}
