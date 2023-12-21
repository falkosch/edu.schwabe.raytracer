#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <ASizeT Index>
  const Float_32 component(const PackedFloat8_256 &v) noexcept;

  template <>
  const Float_32 component<VectorIndices::X1>(const PackedFloat8_256 &v) noexcept;

  template <>
  const Float_32 component<VectorIndices::X2>(const PackedFloat8_256 &v) noexcept;

  template <>
  const Float_32 component<VectorIndices::X3>(const PackedFloat8_256 &v) noexcept;

  template <>
  const Float_32 component<VectorIndices::X4>(const PackedFloat8_256 &v) noexcept;

  template <>
  const Float_32 component<VectorIndices::X5>(const PackedFloat8_256 &v) noexcept;

  template <>
  const Float_32 component<VectorIndices::X6>(const PackedFloat8_256 &v) noexcept;

  template <>
  const Float_32 component<VectorIndices::X7>(const PackedFloat8_256 &v) noexcept;

  template <>
  const Float_32 component<VectorIndices::X8>(const PackedFloat8_256 &v) noexcept;

  const Float_32 x(const PackedFloat8_256 &v) noexcept;
  const Float_32 y(const PackedFloat8_256 &v) noexcept;
  const Float_32 z(const PackedFloat8_256 &v) noexcept;
  const Float_32 w(const PackedFloat8_256 &v) noexcept;

  const Float_32 x1(const PackedFloat8_256 &v) noexcept;
  const Float_32 x2(const PackedFloat8_256 &v) noexcept;
  const Float_32 x3(const PackedFloat8_256 &v) noexcept;
  const Float_32 x4(const PackedFloat8_256 &v) noexcept;
  const Float_32 x5(const PackedFloat8_256 &v) noexcept;
  const Float_32 x6(const PackedFloat8_256 &v) noexcept;
  const Float_32 x7(const PackedFloat8_256 &v) noexcept;
  const Float_32 x8(const PackedFloat8_256 &v) noexcept;
}
