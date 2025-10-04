#include "vectorization/constants/masks/MaskAll.h"

#include "vectorization/accessors.h"

namespace vectorization {
  template <>
  bool MaskAll<bool>() noexcept {
    return true;
  }

  template <>
  Int_8 MaskAll<Int_8>() noexcept {
    return -1;
  }

  template <>
  UInt_8 MaskAll<UInt_8>() noexcept {
    return static_cast<UInt_8>(MaskAll<Int_8>());
  }

  template <>
  Int_16 MaskAll<Int_16>() noexcept {
    return -1;
  }

  template <>
  UInt_16 MaskAll<UInt_16>() noexcept {
    return static_cast<UInt_16>(MaskAll<Int_16>());
  }

  template <>
  Int_32 MaskAll<Int_32>() noexcept {
    return -1;
  }

  template <>
  UInt_32 MaskAll<UInt_32>() noexcept {
    return static_cast<UInt_32>(MaskAll<Int_32>());
  }

  template <>
  Int_64 MaskAll<Int_64>() noexcept {
    return -1;
  }

  template <>
  UInt_64 MaskAll<UInt_64>() noexcept {
    return static_cast<UInt_64>(MaskAll<Int_64>());
  }

  template <>
  Float_32 MaskAll<Float_32>() noexcept {
    return x(MaskAll<PackedFloat4_128>());
  }

  template <>
  Float_64 MaskAll<Float_64>() noexcept {
    return x(MaskAll<PackedFloat2_128>());
  }

  template <>
  PackedInts_128 MaskAll<PackedInts_128>() noexcept {
    return _mm_cmpeq_epi32(_mm_setzero_si128(), _mm_setzero_si128());
  }

  template <>
  PackedFloat4_128 MaskAll<PackedFloat4_128>() noexcept {
    return _mm_castsi128_ps(MaskAll<PackedInts_128>());
  }

  template <>
  PackedFloat2_128 MaskAll<PackedFloat2_128>() noexcept {
    return _mm_castsi128_pd(MaskAll<PackedInts_128>());
  }

  template <>
  PackedInts_256 MaskAll<PackedInts_256>() noexcept {
    return _mm256_set1_epi64x(-1);
  }

  template <>
  PackedFloat8_256 MaskAll<PackedFloat8_256>() noexcept {
    return _mm256_castsi256_ps(MaskAll<PackedInts_256>());
  }

  template <>
  PackedFloat4_256 MaskAll<PackedFloat4_256>() noexcept {
    return _mm256_castsi256_pd(MaskAll<PackedInts_256>());
  }
}
