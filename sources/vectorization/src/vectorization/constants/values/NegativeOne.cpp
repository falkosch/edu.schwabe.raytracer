#include "vectorization/constants/values/NegativeOne.h"

#include "vectorization/constants/values/NegativeZero.h"
#include "vectorization/constants/values/One.h"

#include "vectorization/accessors.h"

namespace vectorization {
  template <>
  const Int_8 NegativeOne() noexcept {
    return Int_8{-1};
  }

  template <>
  const Int_16 NegativeOne() noexcept {
    return Int_16{-1};
  }

  template <>
  const Int_32 NegativeOne() noexcept {
    return Int_32{-1};
  }

  template <>
  const Int_64 NegativeOne() noexcept {
    return Int_64{-1};
  }

  template <>
  const Float_32 NegativeOne<Float_32>() noexcept {
    return x(NegativeOne<PackedFloat4_128>());
  }

  template <>
  const Float_64 NegativeOne<Float_64>() noexcept {
    return x(NegativeOne<PackedFloat2_128>());
  }

  template <>
  const PackedFloat2_128 NegativeOne<PackedFloat2_128>() noexcept {
    return _mm_or_pd(One<PackedFloat2_128>(), NegativeZero<PackedFloat2_128>());
  }

  template <>
  const PackedFloat4_128 NegativeOne<PackedFloat4_128>() noexcept {
    return _mm_or_ps(One<PackedFloat4_128>(), NegativeZero<PackedFloat4_128>());
  }

  template <>
  const PackedFloat4_256 NegativeOne<PackedFloat4_256>() noexcept {
    return _mm256_or_pd(One<PackedFloat4_256>(), NegativeZero<PackedFloat4_256>());
  }

  template <>
  const PackedFloat8_256 NegativeOne<PackedFloat8_256>() noexcept {
    return _mm256_or_ps(One<PackedFloat8_256>(), NegativeZero<PackedFloat8_256>());
  }
}