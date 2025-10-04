#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <typename T>
  T Zero() noexcept;

  template <>
  bool Zero() noexcept;

  template <>
  Int_8 Zero<Int_8>() noexcept;

  template <>
  UInt_8 Zero<UInt_8>() noexcept;

  template <>
  Int_16 Zero<Int_16>() noexcept;

  template <>
  UInt_16 Zero<UInt_16>() noexcept;

  template <>
  Int_32 Zero<Int_32>() noexcept;

  template <>
  UInt_32 Zero<UInt_32>() noexcept;

  template <>
  Int_64 Zero<Int_64>() noexcept;

  template <>
  UInt_64 Zero<UInt_64>() noexcept;

  template <>
  Float_32 Zero<Float_32>() noexcept;

  template <>
  Float_64 Zero<Float_64>() noexcept;

  template <>
  PackedInts_128 Zero<PackedInts_128>() noexcept;

  template <>
  PackedFloat4_128 Zero<PackedFloat4_128>() noexcept;

  template <>
  PackedFloat2_128 Zero<PackedFloat2_128>() noexcept;

  template <>
  PackedInts_256 Zero<PackedInts_256>() noexcept;

  template <>
  PackedFloat8_256 Zero<PackedFloat8_256>() noexcept;

  template <>
  PackedFloat4_256 Zero<PackedFloat4_256>() noexcept;
}
