#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <ASizeT Index>
  PackedFloat2_128 replaceComponent(const PackedFloat2_128 &values, Float_64 replacement) noexcept;

  template <>
  PackedFloat2_128 replaceComponent<VectorIndices::X>(const PackedFloat2_128 &values, Float_64 replacement) noexcept;

  template <>
  PackedFloat2_128 replaceComponent<VectorIndices::Y>(const PackedFloat2_128 &values, Float_64 replacement) noexcept;

  PackedFloat2_128 replaceX(const PackedFloat2_128 &values, Float_64 replacement) noexcept;
  PackedFloat2_128 replaceY(const PackedFloat2_128 &values, Float_64 replacement) noexcept;

  PackedFloat2_128 replaceX1(const PackedFloat2_128 &values, Float_64 replacement) noexcept;
  PackedFloat2_128 replaceX2(const PackedFloat2_128 &values, Float_64 replacement) noexcept;

  PackedFloat2_128
  replaceComponent(const PackedFloat2_128 &values, const PackedFloat2_128 &replacement, ASizeT index) noexcept;
  PackedFloat2_128 replaceComponent(const PackedFloat2_128 &values, Float_64 replacement, ASizeT index) noexcept;
}
