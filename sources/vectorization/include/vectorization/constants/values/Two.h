#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <typename T>
  const T Two() noexcept;

  template <>
  const Int_8 Two<Int_8>() noexcept;

  template <>
  const UInt_8 Two<UInt_8>() noexcept;

  template <>
  const Int_16 Two<Int_16>() noexcept;

  template <>
  const UInt_16 Two<UInt_16>() noexcept;

  template <>
  const Int_32 Two<Int_32>() noexcept;

  template <>
  const UInt_32 Two<UInt_32>() noexcept;

  template <>
  const Int_64 Two<Int_64>() noexcept;

  template <>
  const UInt_64 Two<UInt_64>() noexcept;

  template <>
  const Float_32 Two<Float_32>() noexcept;

  template <>
  const Float_64 Two<Float_64>() noexcept;

  template <>
  const PackedFloat4_128 Two<PackedFloat4_128>() noexcept;

  template <>
  const PackedFloat2_128 Two<PackedFloat2_128>() noexcept;

  template <>
  const PackedFloat8_256 Two<PackedFloat8_256>() noexcept;

  template <>
  const PackedFloat4_256 Two<PackedFloat4_256>() noexcept;
}
