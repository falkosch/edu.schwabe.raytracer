#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <ASizeT Index>
  Int_32 component(const PackedInts_128 &values) noexcept;

  template <>
  Int_32 component<VectorIndices::X>(const PackedInts_128 &values) noexcept;

  template <>
  Int_32 component<VectorIndices::Y>(const PackedInts_128 &values) noexcept;

  template <>
  Int_32 component<VectorIndices::Z>(const PackedInts_128 &values) noexcept;

  template <>
  Int_32 component<VectorIndices::W>(const PackedInts_128 &values) noexcept;

  Int_32 x(const PackedInts_128 &values) noexcept;
  Int_32 y(const PackedInts_128 &values) noexcept;
  Int_32 z(const PackedInts_128 &values) noexcept;
  Int_32 w(const PackedInts_128 &values) noexcept;

  Int_32 x1(const PackedInts_128 &values) noexcept;
  Int_32 x2(const PackedInts_128 &values) noexcept;
  Int_32 x3(const PackedInts_128 &values) noexcept;
  Int_32 x4(const PackedInts_128 &values) noexcept;
}
