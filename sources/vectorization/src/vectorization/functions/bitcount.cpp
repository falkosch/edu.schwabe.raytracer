#include "vectorization/functions/bitCount.h"

namespace vectorization {
  ASizeT bitCount(const bool value) noexcept {
    return value ? ASizeT{1} : ASizeT{0};
  }

  ASizeT bitCount(const Int_8 value) noexcept {
    return bitCount(static_cast<UInt_8>(value));
  }

  ASizeT bitCount(const UInt_8 value) noexcept {
    return bitCount(static_cast<UInt_32>(value) & UInt_32{0xFF});
  }

  ASizeT bitCount(const Int_16 value) noexcept {
    return bitCount(static_cast<UInt_16>(value));
  }

  ASizeT bitCount(const UInt_16 value) noexcept {
    return bitCount(static_cast<UInt_32>(value) & UInt_32{0xFFFF});
  }

  ASizeT bitCount(const Int_32 value) noexcept {
    return bitCount(static_cast<UInt_32>(value));
  }

  ASizeT bitCount(const UInt_32 value) noexcept {
    return static_cast<ASizeT>(_mm_popcnt_u32(value));
  }

  ASizeT bitCount(const Int_64 value) noexcept {
    return bitCount(static_cast<UInt_64>(value));
  }

  ASizeT bitCount(const UInt_64 value) noexcept {
    return static_cast<ASizeT>(_mm_popcnt_u64(value));
  }

  ASizeT bitCount(const Float_32 value) noexcept {
    return bitCount(_mm_set_ss(value));
  }

  ASizeT bitCount(const Float_64 value) noexcept {
    return bitCount(_mm_set_sd(value));
  }

  ASizeT bitCount(const PackedInts_128 &values) noexcept {
    return static_cast<ASizeT>(_mm_popcnt_si128(values));
  }

  ASizeT bitCount(const PackedFloat2_128 &values) noexcept {
    return static_cast<ASizeT>(_mm_popcnt_pd(values));
  }

  ASizeT bitCount(const PackedFloat4_128 &values) noexcept {
    return static_cast<ASizeT>(_mm_popcnt_ps(values));
  }

  ASizeT bitCount(const PackedInts_256 &values) noexcept {
    return static_cast<ASizeT>(_mm256_popcnt_si256(values));
  }

  ASizeT bitCount(const PackedFloat4_256 &values) noexcept {
    return static_cast<ASizeT>(_mm256_popcnt_pd(values));
  }

  ASizeT bitCount(const PackedFloat8_256 &values) noexcept {
    return static_cast<ASizeT>(_mm256_popcnt_ps(values));
  }
}
