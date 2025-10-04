#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <ASizeT Index>
  const PackedFloat4_128 replaceComponent(const PackedFloat4_128 &v, const Float_32 s) noexcept;

  template <>
  const PackedFloat4_128 replaceComponent<VectorIndices::X>(const PackedFloat4_128 &v, const Float_32 s) noexcept;

  template <>
  const PackedFloat4_128 replaceComponent<VectorIndices::Y>(const PackedFloat4_128 &v, const Float_32 s) noexcept;

  template <>
  const PackedFloat4_128 replaceComponent<VectorIndices::Z>(const PackedFloat4_128 &v, const Float_32 s) noexcept;

  template <>
  const PackedFloat4_128 replaceComponent<VectorIndices::W>(const PackedFloat4_128 &v, const Float_32 s) noexcept;

  const PackedFloat4_128 replaceX(const PackedFloat4_128 &v, const Float_32 s) noexcept;
  const PackedFloat4_128 replaceY(const PackedFloat4_128 &v, const Float_32 s) noexcept;
  const PackedFloat4_128 replaceZ(const PackedFloat4_128 &v, const Float_32 s) noexcept;
  const PackedFloat4_128 replaceW(const PackedFloat4_128 &v, const Float_32 s) noexcept;

  const PackedFloat4_128 replaceX1(const PackedFloat4_128 &v, const Float_32 s) noexcept;
  const PackedFloat4_128 replaceX2(const PackedFloat4_128 &v, const Float_32 s) noexcept;
  const PackedFloat4_128 replaceX3(const PackedFloat4_128 &v, const Float_32 s) noexcept;
  const PackedFloat4_128 replaceX4(const PackedFloat4_128 &v, const Float_32 s) noexcept;

  const PackedFloat4_128
  replaceComponent(const PackedFloat4_128 &v, const PackedFloat4_128 &replacement, const ASizeT index) noexcept;
  const PackedFloat4_128
  replaceComponent(const PackedFloat4_128 &v, const Float_32 replacement, const ASizeT index) noexcept;
}
