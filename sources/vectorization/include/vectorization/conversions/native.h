#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <typename U>
  U convert(bool v) noexcept;

  template <typename U>
  U convert(bool a, bool b) noexcept;

  template <typename U>
  U convert(Int_32 v) noexcept;

  template <>
  Int_8 convert<Int_8>(Int_32 v) noexcept;

  template <>
  UInt_8 convert<UInt_8>(Int_32 v) noexcept;

  template <>
  Int_16 convert<Int_16>(Int_32 v) noexcept;

  template <>
  UInt_16 convert<UInt_16>(Int_32 v) noexcept;

  template <>
  Int_32 convert<Int_32>(Int_32 v) noexcept;

  template <>
  UInt_32 convert<UInt_32>(Int_32 v) noexcept;

  template <>
  Int_64 convert<Int_64>(Int_32 v) noexcept;

  template <>
  UInt_64 convert<UInt_64>(Int_32 v) noexcept;

  template <>
  Float_32 convert<Float_32>(Int_32 v) noexcept;

  template <>
  Float_64 convert<Float_64>(Int_32 v) noexcept;

  template <typename U>
  U convert(UInt_32 v) noexcept;

  template <>
  Int_32 convert<Int_32>(UInt_32 v) noexcept;

  template <>
  Int_64 convert<Int_64>(UInt_32 v) noexcept;

  template <>
  UInt_64 convert<UInt_64>(UInt_32 v) noexcept;

  template <>
  Float_32 convert<Float_32>(UInt_32 v) noexcept;

  template <>
  Float_64 convert<Float_64>(UInt_32 v) noexcept;

  template <typename U>
  U convert(Int_64 v) noexcept;

  template <>
  Int_32 convert<Int_32>(Int_64 v) noexcept;

  template <>
  UInt_32 convert<UInt_32>(Int_64 v) noexcept;

  template <>
  UInt_64 convert<UInt_64>(Int_64 v) noexcept;

  template <>
  Float_32 convert<Float_32>(Int_64 v) noexcept;

  template <>
  Float_64 convert<Float_64>(Int_64 v) noexcept;

  template <typename U>
  U convert(UInt_64 v) noexcept;

  template <>
  Int_32 convert<Int_32>(UInt_64 v) noexcept;

  template <>
  UInt_32 convert<UInt_32>(UInt_64 v) noexcept;

  template <>
  Int_64 convert<Int_64>(UInt_64 v) noexcept;

  template <>
  Float_32 convert<Float_32>(UInt_64 v) noexcept;

  template <>
  Float_64 convert<Float_64>(UInt_64 v) noexcept;

  template <typename U>
  U convert(Float_32 v) noexcept;

  template <>
  Int_32 convert<Int_32>(Float_32 v) noexcept;

  template <>
  UInt_32 convert<UInt_32>(Float_32 v) noexcept;

  template <>
  Int_64 convert<Int_64>(Float_32 v) noexcept;

  template <>
  UInt_64 convert<UInt_64>(Float_32 v) noexcept;

  template <>
  Float_64 convert<Float_64>(Float_32 v) noexcept;

  template <typename U>
  U convert(Float_64 v) noexcept;

  template <>
  Int_32 convert<Int_32>(Float_64 v) noexcept;

  template <>
  UInt_32 convert<UInt_32>(Float_64 v) noexcept;

  template <>
  Int_64 convert<Int_64>(Float_64 v) noexcept;

  template <>
  UInt_64 convert<UInt_64>(Float_64 v) noexcept;

  template <>
  Float_32 convert<Float_32>(Float_64 v) noexcept;
}
