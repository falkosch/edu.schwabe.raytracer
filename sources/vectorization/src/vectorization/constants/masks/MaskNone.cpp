#include "vectorization/constants/masks/MaskNone.h"

#include "vectorization/accessors.h"

namespace vectorization {
  template <>
  bool MaskNone<bool>() noexcept {
    return false;
  }

  template <>
  Int_8 MaskNone<Int_8>() noexcept {
    return 0;
  }

  template <>
  UInt_8 MaskNone<UInt_8>() noexcept {
    return 0;
  }

  template <>
  Int_16 MaskNone<Int_16>() noexcept {
    return 0;
  }

  template <>
  UInt_16 MaskNone<UInt_16>() noexcept {
    return 0;
  }

  template <>
  Int_32 MaskNone<Int_32>() noexcept {
    return 0;
  }

  template <>
  UInt_32 MaskNone<UInt_32>() noexcept {
    return 0;
  }

  template <>
  Int_64 MaskNone<Int_64>() noexcept {
    return 0;
  }

  template <>
  UInt_64 MaskNone<UInt_64>() noexcept {
    return 0;
  }

  template <>
  Float_32 MaskNone<Float_32>() noexcept {
    return x(MaskNone<PackedFloat4_128>());
  }

  template <>
  Float_64 MaskNone<Float_64>() noexcept {
    return x(MaskNone<PackedFloat2_128>());
  }

  template <>
  PackedInts_128 MaskNone<PackedInts_128>() noexcept {
    return _mm_setzero_si128();
  }

  template <>
  PackedFloat4_128 MaskNone<PackedFloat4_128>() noexcept {
    return _mm_setzero_ps();
  }

  template <>
  PackedFloat2_128 MaskNone<PackedFloat2_128>() noexcept {
    return _mm_setzero_pd();
  }

  template <>
  PackedInts_256 MaskNone<PackedInts_256>() noexcept {
    return _mm256_setzero_si256();
  }

  template <>
  PackedFloat8_256 MaskNone<PackedFloat8_256>() noexcept {
    return _mm256_setzero_ps();
  }

  template <>
  PackedFloat4_256 MaskNone<PackedFloat4_256>() noexcept {
    return _mm256_setzero_pd();
  }
}
