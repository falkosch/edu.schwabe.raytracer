#include "vectorization/constants/values/One.h"

#include "vectorization/constants/masks.h"

#include "vectorization/accessors.h"

namespace vectorization {
  template <>
  bool One() noexcept {
    return true;
  }

  template <>
  Int_8 One() noexcept {
    return Int_8{1};
  }

  template <>
  UInt_8 One() noexcept {
    return UInt_8{1};
  }

  template <>
  Int_16 One() noexcept {
    return Int_16{1};
  }

  template <>
  UInt_16 One() noexcept {
    return UInt_16{1};
  }

  template <>
  Int_32 One() noexcept {
    return Int_32{1};
  }

  template <>
  UInt_32 One() noexcept {
    return UInt_32{1};
  }

  template <>
  Int_64 One() noexcept {
    return Int_64{1};
  }

  template <>
  UInt_64 One() noexcept {
    return UInt_64{1};
  }

  template <>
  Float_32 One<Float_32>() noexcept {
    return x(One<PackedFloat4_128>());
  }

  template <>
  Float_64 One<Float_64>() noexcept {
    return x(One<PackedFloat2_128>());
  }

  template <>
  PackedFloat2_128 One<PackedFloat2_128>() noexcept {
    return _mm_castsi128_pd(_mm_srli_epi64(_mm_slli_epi64(MaskAll<PackedInts_128>(), 54), 2));
  }

  template <>
  PackedFloat4_128 One<PackedFloat4_128>() noexcept {
    return _mm_castsi128_ps(_mm_srli_epi32(_mm_slli_epi32(MaskAll<PackedInts_128>(), 25), 2));
  }

  template <>
  PackedFloat4_256 One<PackedFloat4_256>() noexcept {
    const auto v = One<PackedFloat2_128>();
    return _mm256_set_m128d(v, v);
  }

  template <>
  PackedFloat8_256 One<PackedFloat8_256>() noexcept {
    const auto v = One<PackedFloat4_128>();
    return _mm256_set_m128(v, v);
  }
}
