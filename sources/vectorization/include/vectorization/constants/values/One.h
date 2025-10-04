#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <typename T>
  T One() noexcept;

  template <>
  bool One<bool>() noexcept;

  template <>
  Int_8 One<Int_8>() noexcept;

  template <>
  UInt_8 One<UInt_8>() noexcept;

  template <>
  Int_16 One<Int_16>() noexcept;

  template <>
  UInt_16 One<UInt_16>() noexcept;

  template <>
  Int_32 One<Int_32>() noexcept;

  template <>
  UInt_32 One<UInt_32>() noexcept;

  template <>
  Int_64 One<Int_64>() noexcept;

  template <>
  UInt_64 One<UInt_64>() noexcept;

  template <>
  Float_32 One<Float_32>() noexcept;

  template <>
  Float_64 One<Float_64>() noexcept;

  template <>
  PackedFloat4_128 One<PackedFloat4_128>() noexcept;

  template <>
  PackedFloat2_128 One<PackedFloat2_128>() noexcept;

  template <>
  PackedFloat8_256 One<PackedFloat8_256>() noexcept;

  template <>
  PackedFloat4_256 One<PackedFloat4_256>() noexcept;
}
