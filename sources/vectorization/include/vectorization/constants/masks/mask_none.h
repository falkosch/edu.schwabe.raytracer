#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  // Convenience function for MaskAll. Return is equivalent to 0.
  template <typename V>
  V MaskNone() noexcept;

  template <>
  bool MaskNone<bool>() noexcept;

  template <>
  Int_8 MaskNone<Int_8>() noexcept;

  template <>
  UInt_8 MaskNone<UInt_8>() noexcept;

  template <>
  Int_16 MaskNone<Int_16>() noexcept;

  template <>
  UInt_16 MaskNone<UInt_16>() noexcept;

  template <>
  Int_32 MaskNone<Int_32>() noexcept;

  template <>
  UInt_32 MaskNone<UInt_32>() noexcept;

  template <>
  Int_64 MaskNone<Int_64>() noexcept;

  template <>
  UInt_64 MaskNone<UInt_64>() noexcept;

  template <>
  Float_32 MaskNone<Float_32>() noexcept;

  template <>
  Float_64 MaskNone<Float_64>() noexcept;

  template <>
  PackedInts_128 MaskNone<PackedInts_128>() noexcept;

  template <>
  PackedFloat4_128 MaskNone<PackedFloat4_128>() noexcept;

  template <>
  PackedFloat2_128 MaskNone<PackedFloat2_128>() noexcept;

  template <>
  PackedInts_256 MaskNone<PackedInts_256>() noexcept;

  template <>
  PackedFloat8_256 MaskNone<PackedFloat8_256>() noexcept;

  template <>
  PackedFloat4_256 MaskNone<PackedFloat4_256>() noexcept;
}
