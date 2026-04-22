#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <ASizeT Index>
  PackedInts_128 replaceComponent(const PackedInts_128 &values, Int_32 replacement) noexcept;

  template <>
  PackedInts_128 replaceComponent<VectorIndices::X>(const PackedInts_128 &values, Int_32 replacement) noexcept;

  template <>
  PackedInts_128 replaceComponent<VectorIndices::Y>(const PackedInts_128 &values, Int_32 replacement) noexcept;

  template <>
  PackedInts_128 replaceComponent<VectorIndices::Z>(const PackedInts_128 &values, Int_32 replacement) noexcept;

  template <>
  PackedInts_128 replaceComponent<VectorIndices::W>(const PackedInts_128 &values, Int_32 replacement) noexcept;

  PackedInts_128 replaceX(const PackedInts_128 &values, Int_32 replacement) noexcept;
  PackedInts_128 replaceY(const PackedInts_128 &values, Int_32 replacement) noexcept;
  PackedInts_128 replaceZ(const PackedInts_128 &values, Int_32 replacement) noexcept;
  PackedInts_128 replaceW(const PackedInts_128 &values, Int_32 replacement) noexcept;

  PackedInts_128 replaceX1(const PackedInts_128 &values, Int_32 replacement) noexcept;
  PackedInts_128 replaceX2(const PackedInts_128 &values, Int_32 replacement) noexcept;
  PackedInts_128 replaceX3(const PackedInts_128 &values, Int_32 replacement) noexcept;
  PackedInts_128 replaceX4(const PackedInts_128 &values, Int_32 replacement) noexcept;

  PackedInts_128
  replaceComponent(const PackedInts_128 &values, const PackedInts_128 &replacement, ASizeT index) noexcept;
  PackedInts_128 replaceComponent(const PackedInts_128 &values, Int_32 replacement, ASizeT index) noexcept;
}
