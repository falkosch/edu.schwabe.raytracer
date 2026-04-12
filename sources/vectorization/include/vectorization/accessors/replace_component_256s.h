#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <ASizeT Index>
  PackedFloat8_256 replaceComponent(const PackedFloat8_256 &values, Float_32 replacement) noexcept;

  template <>
  PackedFloat8_256 replaceComponent<VectorIndices::X1>(const PackedFloat8_256 &values, Float_32 replacement) noexcept;

  template <>
  PackedFloat8_256 replaceComponent<VectorIndices::X2>(const PackedFloat8_256 &values, Float_32 replacement) noexcept;

  template <>
  PackedFloat8_256 replaceComponent<VectorIndices::X3>(const PackedFloat8_256 &values, Float_32 replacement) noexcept;

  template <>
  PackedFloat8_256 replaceComponent<VectorIndices::X4>(const PackedFloat8_256 &values, Float_32 replacement) noexcept;

  template <>
  PackedFloat8_256 replaceComponent<VectorIndices::X5>(const PackedFloat8_256 &values, Float_32 replacement) noexcept;

  template <>
  PackedFloat8_256 replaceComponent<VectorIndices::X6>(const PackedFloat8_256 &values, Float_32 replacement) noexcept;

  template <>
  PackedFloat8_256 replaceComponent<VectorIndices::X7>(const PackedFloat8_256 &values, Float_32 replacement) noexcept;

  template <>
  PackedFloat8_256 replaceComponent<VectorIndices::X8>(const PackedFloat8_256 &values, Float_32 replacement) noexcept;

  PackedFloat8_256 replaceX(const PackedFloat8_256 &values, Float_32 replacement) noexcept;
  PackedFloat8_256 replaceY(const PackedFloat8_256 &values, Float_32 replacement) noexcept;
  PackedFloat8_256 replaceZ(const PackedFloat8_256 &values, Float_32 replacement) noexcept;
  PackedFloat8_256 replaceW(const PackedFloat8_256 &values, Float_32 replacement) noexcept;

  PackedFloat8_256 replaceX1(const PackedFloat8_256 &values, Float_32 replacement) noexcept;
  PackedFloat8_256 replaceX2(const PackedFloat8_256 &values, Float_32 replacement) noexcept;
  PackedFloat8_256 replaceX3(const PackedFloat8_256 &values, Float_32 replacement) noexcept;
  PackedFloat8_256 replaceX4(const PackedFloat8_256 &values, Float_32 replacement) noexcept;
  PackedFloat8_256 replaceX5(const PackedFloat8_256 &values, Float_32 replacement) noexcept;
  PackedFloat8_256 replaceX6(const PackedFloat8_256 &values, Float_32 replacement) noexcept;
  PackedFloat8_256 replaceX7(const PackedFloat8_256 &values, Float_32 replacement) noexcept;
  PackedFloat8_256 replaceX8(const PackedFloat8_256 &values, Float_32 replacement) noexcept;

  PackedFloat8_256
  replaceComponent(const PackedFloat8_256 &values, const PackedFloat8_256 &replacement, ASizeT index) noexcept;
  PackedFloat8_256 replaceComponent(const PackedFloat8_256 &values, Float_32 replacement, ASizeT index) noexcept;
}
