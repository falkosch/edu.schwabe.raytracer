#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <typename T>
  T NegativeTwo() noexcept;

  template <>
  Int_8 NegativeTwo<Int_8>() noexcept;

  template <>
  Int_16 NegativeTwo<Int_16>() noexcept;

  template <>
  Int_32 NegativeTwo<Int_32>() noexcept;

  template <>
  Int_64 NegativeTwo<Int_64>() noexcept;

  template <>
  Float_32 NegativeTwo<Float_32>() noexcept;

  template <>
  Float_64 NegativeTwo<Float_64>() noexcept;

  template <>
  PackedFloat4_128 NegativeTwo<PackedFloat4_128>() noexcept;

  template <>
  PackedFloat2_128 NegativeTwo<PackedFloat2_128>() noexcept;

  template <>
  PackedFloat8_256 NegativeTwo<PackedFloat8_256>() noexcept;

  template <>
  PackedFloat4_256 NegativeTwo<PackedFloat4_256>() noexcept;
}
