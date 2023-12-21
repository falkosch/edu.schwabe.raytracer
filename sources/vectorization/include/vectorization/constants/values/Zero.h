#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <typename T>
  const T Zero() noexcept;

  template <>
  const bool Zero() noexcept;

  template <>
  const Int_8 Zero<Int_8>() noexcept;

  template <>
  const UInt_8 Zero<UInt_8>() noexcept;

  template <>
  const Int_16 Zero<Int_16>() noexcept;

  template <>
  const UInt_16 Zero<UInt_16>() noexcept;

  template <>
  const Int_32 Zero<Int_32>() noexcept;

  template <>
  const UInt_32 Zero<UInt_32>() noexcept;

  template <>
  const Int_64 Zero<Int_64>() noexcept;

  template <>
  const UInt_64 Zero<UInt_64>() noexcept;

  template <>
  const Float_32 Zero<Float_32>() noexcept;

  template <>
  const Float_64 Zero<Float_64>() noexcept;

  template <>
  const PackedInts_128 Zero<PackedInts_128>() noexcept;

  template <>
  const PackedFloat4_128 Zero<PackedFloat4_128>() noexcept;

  template <>
  const PackedFloat2_128 Zero<PackedFloat2_128>() noexcept;

  template <>
  const PackedInts_256 Zero<PackedInts_256>() noexcept;

  template <>
  const PackedFloat8_256 Zero<PackedFloat8_256>() noexcept;

  template <>
  const PackedFloat4_256 Zero<PackedFloat4_256>() noexcept;
}
