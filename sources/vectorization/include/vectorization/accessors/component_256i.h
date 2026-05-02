#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <ASizeT Index>
  Int_32 component(const PackedInts_256 &values) noexcept;

  template <>
  Int_32 component<VectorIndices::X1>(const PackedInts_256 &values) noexcept;

  template <>
  Int_32 component<VectorIndices::X2>(const PackedInts_256 &values) noexcept;

  template <>
  Int_32 component<VectorIndices::X3>(const PackedInts_256 &values) noexcept;

  template <>
  Int_32 component<VectorIndices::X4>(const PackedInts_256 &values) noexcept;

  template <>
  Int_32 component<VectorIndices::X5>(const PackedInts_256 &values) noexcept;

  template <>
  Int_32 component<VectorIndices::X6>(const PackedInts_256 &values) noexcept;

  template <>
  Int_32 component<VectorIndices::X7>(const PackedInts_256 &values) noexcept;

  template <>
  Int_32 component<VectorIndices::X8>(const PackedInts_256 &values) noexcept;

  Int_32 x1(const PackedInts_256 &values) noexcept;
  Int_32 x2(const PackedInts_256 &values) noexcept;
  Int_32 x3(const PackedInts_256 &values) noexcept;
  Int_32 x4(const PackedInts_256 &values) noexcept;
  Int_32 x5(const PackedInts_256 &values) noexcept;
  Int_32 x6(const PackedInts_256 &values) noexcept;
  Int_32 x7(const PackedInts_256 &values) noexcept;
  Int_32 x8(const PackedInts_256 &values) noexcept;
}
