#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <ASizeT Index>
  PackedInts_256 replaceComponent(const PackedInts_256 &values, Int_32 replacement) noexcept;

  template <>
  PackedInts_256 replaceComponent<VectorIndices::X1>(const PackedInts_256 &values, Int_32 replacement) noexcept;

  template <>
  PackedInts_256 replaceComponent<VectorIndices::X2>(const PackedInts_256 &values, Int_32 replacement) noexcept;

  template <>
  PackedInts_256 replaceComponent<VectorIndices::X3>(const PackedInts_256 &values, Int_32 replacement) noexcept;

  template <>
  PackedInts_256 replaceComponent<VectorIndices::X4>(const PackedInts_256 &values, Int_32 replacement) noexcept;

  template <>
  PackedInts_256 replaceComponent<VectorIndices::X5>(const PackedInts_256 &values, Int_32 replacement) noexcept;

  template <>
  PackedInts_256 replaceComponent<VectorIndices::X6>(const PackedInts_256 &values, Int_32 replacement) noexcept;

  template <>
  PackedInts_256 replaceComponent<VectorIndices::X7>(const PackedInts_256 &values, Int_32 replacement) noexcept;

  template <>
  PackedInts_256 replaceComponent<VectorIndices::X8>(const PackedInts_256 &values, Int_32 replacement) noexcept;

  PackedInts_256 replaceX1(const PackedInts_256 &values, Int_32 replacement) noexcept;
  PackedInts_256 replaceX2(const PackedInts_256 &values, Int_32 replacement) noexcept;
  PackedInts_256 replaceX3(const PackedInts_256 &values, Int_32 replacement) noexcept;
  PackedInts_256 replaceX4(const PackedInts_256 &values, Int_32 replacement) noexcept;
  PackedInts_256 replaceX5(const PackedInts_256 &values, Int_32 replacement) noexcept;
  PackedInts_256 replaceX6(const PackedInts_256 &values, Int_32 replacement) noexcept;
  PackedInts_256 replaceX7(const PackedInts_256 &values, Int_32 replacement) noexcept;
  PackedInts_256 replaceX8(const PackedInts_256 &values, Int_32 replacement) noexcept;

  PackedInts_256
  replaceComponent(const PackedInts_256 &values, const PackedInts_256 &replacement, ASizeT index) noexcept;
  PackedInts_256 replaceComponent(const PackedInts_256 &values, Int_32 replacement, ASizeT index) noexcept;
}
