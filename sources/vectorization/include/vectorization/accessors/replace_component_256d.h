#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <ASizeT Index>
  PackedFloat4_256 replaceComponent(const PackedFloat4_256 &values, Float_64 replacement) noexcept;

  template <>
  PackedFloat4_256 replaceComponent<VectorIndices::X>(const PackedFloat4_256 &values, Float_64 replacement) noexcept;

  template <>
  PackedFloat4_256 replaceComponent<VectorIndices::Y>(const PackedFloat4_256 &values, Float_64 replacement) noexcept;

  template <>
  PackedFloat4_256 replaceComponent<VectorIndices::Z>(const PackedFloat4_256 &values, Float_64 replacement) noexcept;

  template <>
  PackedFloat4_256 replaceComponent<VectorIndices::W>(const PackedFloat4_256 &values, Float_64 replacement) noexcept;

  PackedFloat4_256 replaceX(const PackedFloat4_256 &values, Float_64 replacement) noexcept;
  PackedFloat4_256 replaceY(const PackedFloat4_256 &values, Float_64 replacement) noexcept;
  PackedFloat4_256 replaceZ(const PackedFloat4_256 &values, Float_64 replacement) noexcept;
  PackedFloat4_256 replaceW(const PackedFloat4_256 &values, Float_64 replacement) noexcept;

  PackedFloat4_256 replaceX1(const PackedFloat4_256 &values, Float_64 replacement) noexcept;
  PackedFloat4_256 replaceX2(const PackedFloat4_256 &values, Float_64 replacement) noexcept;
  PackedFloat4_256 replaceX3(const PackedFloat4_256 &values, Float_64 replacement) noexcept;
  PackedFloat4_256 replaceX4(const PackedFloat4_256 &values, Float_64 replacement) noexcept;

  PackedFloat4_256
  replaceComponent(const PackedFloat4_256 &values, const PackedFloat4_256 &replacement, ASizeT index) noexcept;
  PackedFloat4_256 replaceComponent(const PackedFloat4_256 &values, Float_64 replacement, ASizeT index) noexcept;
}
