#pragma once

#include "meta_types.h"

namespace vectorization {
  template <>
  struct PackedTypes<Int_8, 16> {
    static constexpr ASizeT Elements = 16;
    using ValueType = Int_8;
    using Type = PackedInts_128;
  };

  template <>
  struct PackedTypes<Int_8, 32> {
    static constexpr ASizeT Elements = 32;
    using ValueType = Int_8;
    using Type = PackedInts_256;
  };

  template <>
  struct PackedTypes<Int_16, 8> {
    static constexpr ASizeT Elements = 8;
    using ValueType = Int_16;
    using Type = PackedInts_128;
  };

  template <>
  struct PackedTypes<Int_16, 16> {
    static constexpr ASizeT Elements = 16;
    using ValueType = Int_16;
    using Type = PackedInts_256;
  };

  template <>
  struct PackedTypes<Int_32, 4> {
    static constexpr ASizeT Elements = 4;
    using ValueType = Int_32;
    using Type = PackedInts_128;
  };

  template <>
  struct PackedTypes<Int_32, 8> {
    static constexpr ASizeT Elements = 8;
    using ValueType = Int_32;
    using Type = PackedInts_256;
  };

  template <>
  struct PackedTypes<Int_64, 2> {
    static constexpr ASizeT Elements = 2;
    using ValueType = Int_64;
    using Type = PackedInts_128;
  };

  template <>
  struct PackedTypes<Int_64, 4> {
    static constexpr ASizeT Elements = 4;
    using ValueType = Int_64;
    using Type = PackedInts_256;
  };

  template <>
  struct PackedTypes<UInt_8, 16> {
    static constexpr ASizeT Elements = 16;
    using ValueType = UInt_8;
    using Type = PackedInts_128;
  };

  template <>
  struct PackedTypes<UInt_8, 32> {
    static constexpr ASizeT Elements = 32;
    using ValueType = UInt_8;
    using Type = PackedInts_256;
  };

  template <>
  struct PackedTypes<UInt_16, 8> {
    static constexpr ASizeT Elements = 8;
    using ValueType = UInt_16;
    using Type = PackedInts_128;
  };

  template <>
  struct PackedTypes<UInt_16, 16> {
    static constexpr ASizeT Elements = 16;
    using ValueType = UInt_16;
    using Type = PackedInts_256;
  };

  template <>
  struct PackedTypes<UInt_32, 4> {
    static constexpr ASizeT Elements = 4;
    using ValueType = UInt_32;
    using Type = PackedInts_128;
  };

  template <>
  struct PackedTypes<UInt_32, 8> {
    static constexpr ASizeT Elements = 8;
    using ValueType = UInt_32;
    using Type = PackedInts_256;
  };

  template <>
  struct PackedTypes<UInt_64, 2> {
    static constexpr ASizeT Elements = 2;
    using ValueType = UInt_64;
    using Type = PackedInts_128;
  };

  template <>
  struct PackedTypes<UInt_64, 4> {
    static constexpr ASizeT Elements = 4;
    using ValueType = UInt_64;
    using Type = PackedInts_256;
  };

  template <>
  struct PackedTypes<Float_32, 4> {
    static constexpr ASizeT Elements = 4;
    using ValueType = Float_32;
    using Type = PackedFloat4_128;
  };

  template <>
  struct PackedTypes<Float_32, 8> {
    static constexpr ASizeT Elements = 8;
    using ValueType = Float_32;
    using Type = PackedFloat8_256;
  };

  template <>
  struct PackedTypes<Float_64, 2> {
    static constexpr ASizeT Elements = 2;
    using ValueType = Float_64;
    using Type = PackedFloat2_128;
  };

  template <>
  struct PackedTypes<Float_64, 4> {
    static constexpr ASizeT Elements = 4;
    using ValueType = Float_64;
    using Type = PackedFloat4_256;
  };
}
