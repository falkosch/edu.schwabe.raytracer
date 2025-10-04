#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <typename T>
  T Two() noexcept;

  template <>
  Int_8 Two<Int_8>() noexcept;

  template <>
  UInt_8 Two<UInt_8>() noexcept;

  template <>
  Int_16 Two<Int_16>() noexcept;

  template <>
  UInt_16 Two<UInt_16>() noexcept;

  template <>
  Int_32 Two<Int_32>() noexcept;

  template <>
  UInt_32 Two<UInt_32>() noexcept;

  template <>
  Int_64 Two<Int_64>() noexcept;

  template <>
  UInt_64 Two<UInt_64>() noexcept;

  template <>
  Float_32 Two<Float_32>() noexcept;

  template <>
  Float_64 Two<Float_64>() noexcept;

  template <>
  PackedFloat4_128 Two<PackedFloat4_128>() noexcept;

  template <>
  PackedFloat2_128 Two<PackedFloat2_128>() noexcept;

  template <>
  PackedFloat8_256 Two<PackedFloat8_256>() noexcept;

  template <>
  PackedFloat4_256 Two<PackedFloat4_256>() noexcept;
}
