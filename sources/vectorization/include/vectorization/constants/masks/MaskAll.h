#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <typename V>
  V MaskAll() noexcept;

  template <>
  bool MaskAll<bool>() noexcept;

  template <>
  Int_8 MaskAll<Int_8>() noexcept;

  template <>
  UInt_8 MaskAll<UInt_8>() noexcept;

  template <>
  Int_16 MaskAll<Int_16>() noexcept;

  template <>
  UInt_16 MaskAll<UInt_16>() noexcept;

  template <>
  Int_32 MaskAll<Int_32>() noexcept;

  template <>
  UInt_32 MaskAll<UInt_32>() noexcept;

  template <>
  Int_64 MaskAll<Int_64>() noexcept;

  template <>
  UInt_64 MaskAll<UInt_64>() noexcept;

  template <>
  Float_32 MaskAll<Float_32>() noexcept;

  template <>
  Float_64 MaskAll<Float_64>() noexcept;

  template <>
  PackedInts_128 MaskAll<PackedInts_128>() noexcept;

  template <>
  PackedFloat4_128 MaskAll<PackedFloat4_128>() noexcept;

  template <>
  PackedFloat2_128 MaskAll<PackedFloat2_128>() noexcept;

  template <>
  PackedInts_256 MaskAll<PackedInts_256>() noexcept;

  template <>
  PackedFloat8_256 MaskAll<PackedFloat8_256>() noexcept;

  template <>
  PackedFloat4_256 MaskAll<PackedFloat4_256>() noexcept;
}
