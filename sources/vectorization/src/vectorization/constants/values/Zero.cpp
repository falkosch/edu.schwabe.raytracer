#include "vectorization/constants/values/Zero.h"

#include "vectorization/accessors.h"

namespace vectorization {
  template <>
  bool Zero() noexcept {
    return false;
  }

  template <>
  Int_8 Zero() noexcept {
    return Int_8{0};
  }

  template <>
  UInt_8 Zero() noexcept {
    return UInt_8{0};
  }

  template <>
  Int_16 Zero() noexcept {
    return Int_16{0};
  }

  template <>
  UInt_16 Zero() noexcept {
    return UInt_16{0};
  }

  template <>
  Int_32 Zero() noexcept {
    return Int_32{0};
  }

  template <>
  UInt_32 Zero() noexcept {
    return UInt_32{0};
  }

  template <>
  Int_64 Zero() noexcept {
    return Int_64{0};
  }

  template <>
  UInt_64 Zero() noexcept {
    return UInt_64{0};
  }

  template <>
  Float_32 Zero<Float_32>() noexcept {
    return x(Zero<PackedFloat4_128>());
  }

  template <>
  Float_64 Zero<Float_64>() noexcept {
    return x(Zero<PackedFloat2_128>());
  }

  template <>
  PackedInts_128 Zero<PackedInts_128>() noexcept {
    return _mm_setzero_si128();
  }

  template <>
  PackedFloat2_128 Zero<PackedFloat2_128>() noexcept {
    return _mm_setzero_pd();
  }

  template <>
  PackedFloat4_128 Zero<PackedFloat4_128>() noexcept {
    return _mm_setzero_ps();
  }

  template <>
  PackedInts_256 Zero<PackedInts_256>() noexcept {
    return _mm256_setzero_si256();
  }

  template <>
  PackedFloat4_256 Zero<PackedFloat4_256>() noexcept {
    return _mm256_setzero_pd();
  }

  template <>
  PackedFloat8_256 Zero<PackedFloat8_256>() noexcept {
    return _mm256_setzero_ps();
  }
}
