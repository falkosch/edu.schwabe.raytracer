#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <ASizeT Index>
  Float_64 component(const PackedFloat2_128 &values) noexcept;

  template <>
  Float_64 component<VectorIndices::X>(const PackedFloat2_128 &values) noexcept;

  template <>
  Float_64 component<VectorIndices::Y>(const PackedFloat2_128 &values) noexcept;

  Float_64 x(const PackedFloat2_128 &values) noexcept;
  Float_64 y(const PackedFloat2_128 &values) noexcept;

  Float_64 x1(const PackedFloat2_128 &values) noexcept;
  Float_64 x2(const PackedFloat2_128 &values) noexcept;
}
