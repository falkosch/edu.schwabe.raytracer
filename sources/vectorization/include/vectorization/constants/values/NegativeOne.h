#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <typename T>
  T NegativeOne() noexcept;

  template <>
  Int_8 NegativeOne<Int_8>() noexcept;

  template <>
  Int_16 NegativeOne<Int_16>() noexcept;

  template <>
  Int_32 NegativeOne<Int_32>() noexcept;

  template <>
  Int_64 NegativeOne<Int_64>() noexcept;

  template <>
  Float_32 NegativeOne<Float_32>() noexcept;

  template <>
  Float_64 NegativeOne<Float_64>() noexcept;

  template <>
  PackedFloat4_128 NegativeOne<PackedFloat4_128>() noexcept;

  template <>
  PackedFloat2_128 NegativeOne<PackedFloat2_128>() noexcept;

  template <>
  PackedFloat8_256 NegativeOne<PackedFloat8_256>() noexcept;

  template <>
  PackedFloat4_256 NegativeOne<PackedFloat4_256>() noexcept;
}
