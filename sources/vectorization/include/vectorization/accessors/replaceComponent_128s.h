#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <ASizeT Index>
  PackedFloat4_128 replaceComponent(const PackedFloat4_128 &values, Float_32 replacement) noexcept;

  template <>
  PackedFloat4_128 replaceComponent<VectorIndices::X>(const PackedFloat4_128 &values, Float_32 replacement) noexcept;

  template <>
  PackedFloat4_128 replaceComponent<VectorIndices::Y>(const PackedFloat4_128 &values, Float_32 replacement) noexcept;

  template <>
  PackedFloat4_128 replaceComponent<VectorIndices::Z>(const PackedFloat4_128 &values, Float_32 replacement) noexcept;

  template <>
  PackedFloat4_128 replaceComponent<VectorIndices::W>(const PackedFloat4_128 &values, Float_32 replacement) noexcept;

  PackedFloat4_128 replaceX(const PackedFloat4_128 &values, Float_32 replacement) noexcept;
  PackedFloat4_128 replaceY(const PackedFloat4_128 &values, Float_32 replacement) noexcept;
  PackedFloat4_128 replaceZ(const PackedFloat4_128 &values, Float_32 replacement) noexcept;
  PackedFloat4_128 replaceW(const PackedFloat4_128 &values, Float_32 replacement) noexcept;

  PackedFloat4_128 replaceX1(const PackedFloat4_128 &values, Float_32 replacement) noexcept;
  PackedFloat4_128 replaceX2(const PackedFloat4_128 &values, Float_32 replacement) noexcept;
  PackedFloat4_128 replaceX3(const PackedFloat4_128 &values, Float_32 replacement) noexcept;
  PackedFloat4_128 replaceX4(const PackedFloat4_128 &values, Float_32 replacement) noexcept;

  PackedFloat4_128
  replaceComponent(const PackedFloat4_128 &values, const PackedFloat4_128 &replacement, ASizeT index) noexcept;
  PackedFloat4_128 replaceComponent(const PackedFloat4_128 &values, Float_32 replacement, ASizeT index) noexcept;
}
