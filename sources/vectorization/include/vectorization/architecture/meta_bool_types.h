#pragma once

#include "meta_types.h"

namespace vectorization {
  template <>
  struct BoolTypes<Int_8> {
    typedef Int_8 ValueType;
    typedef Bool_8 Type;
  };

  template <>
  struct BoolTypes<Int_16> {
    typedef Int_16 ValueType;
    typedef Bool_16 Type;
  };

  template <>
  struct BoolTypes<Int_32> {
    typedef Int_32 ValueType;
    typedef Bool_32 Type;
  };

  template <>
  struct BoolTypes<Int_64> {
    typedef Int_64 ValueType;
    typedef Bool_64 Type;
  };

  template <>
  struct BoolTypes<UInt_8> {
    typedef UInt_8 ValueType;
    typedef Bool_8 Type;
  };

  template <>
  struct BoolTypes<UInt_16> {
    typedef UInt_16 ValueType;
    typedef Bool_16 Type;
  };

  template <>
  struct BoolTypes<UInt_32> {
    typedef UInt_32 ValueType;
    typedef Bool_32 Type;
  };

  template <>
  struct BoolTypes<UInt_64> {
    typedef UInt_64 ValueType;
    typedef Bool_64 Type;
  };

  template <>
  struct BoolTypes<Float_32> {
    typedef Float_32 ValueType;
    typedef Bool_32 Type;
  };

  template <>
  struct BoolTypes<Float_64> {
    typedef Float_64 ValueType;
    typedef Bool_64 Type;
  };
}
