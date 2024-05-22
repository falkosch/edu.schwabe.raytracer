#pragma once

#include "meta_types.h"

namespace vectorization {
  template <>
  struct BoolTypes<Int_8> {
    using ValueType = Int_8;
    using Type = Bool_8;
  };

  template <>
  struct BoolTypes<Int_16> {
    using ValueType = Int_16;
    using Type = Bool_16;
  };

  template <>
  struct BoolTypes<Int_32> {
    using ValueType = Int_32;
    using Type = Bool_32;
  };

  template <>
  struct BoolTypes<Int_64> {
    using ValueType = Int_64;
    using Type = Bool_64;
  };

  template <>
  struct BoolTypes<UInt_8> {
    using ValueType = UInt_8;
    using Type = Bool_8;
  };

  template <>
  struct BoolTypes<UInt_16> {
    using ValueType = UInt_16;
    using Type = Bool_16;
  };

  template <>
  struct BoolTypes<UInt_32> {
    using ValueType = UInt_32;
    using Type = Bool_32;
  };

  template <>
  struct BoolTypes<UInt_64> {
    using ValueType = UInt_64;
    using Type = Bool_64;
  };

  template <>
  struct BoolTypes<Float_32> {
    using ValueType = Float_32;
    using Type = Bool_32;
  };

  template <>
  struct BoolTypes<Float_64> {
    using ValueType = Float_64;
    using Type = Bool_64;
  };
}
