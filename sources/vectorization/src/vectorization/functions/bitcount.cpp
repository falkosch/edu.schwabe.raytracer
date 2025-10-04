#include "vectorization/functions/bitCount.h"

namespace vectorization {
  const ASizeT bitCount(const bool v) noexcept {
    return v ? ASizeT{1} : ASizeT{0};
  }

  const ASizeT bitCount(const Int_8 v) noexcept {
    return bitCount(static_cast<UInt_8>(v));
  }

  const ASizeT bitCount(const UInt_8 v) noexcept {
    return bitCount(static_cast<UInt_32>(v) & UInt_32{0xFF});
  }

  const ASizeT bitCount(const Int_16 v) noexcept {
    return bitCount(static_cast<UInt_16>(v));
  }

  const ASizeT bitCount(const UInt_16 v) noexcept {
    return bitCount(static_cast<UInt_32>(v) & UInt_32{0xFFFF});
  }

  const ASizeT bitCount(const Int_32 v) noexcept {
    return bitCount(static_cast<UInt_32>(v));
  }

  const ASizeT bitCount(const UInt_32 v) noexcept {
    return static_cast<ASizeT>(_mm_popcnt_u32(v));
  }

  const ASizeT bitCount(const Int_64 v) noexcept {
    return bitCount(static_cast<UInt_64>(v));
  }

  const ASizeT bitCount(const UInt_64 v) noexcept {
    return static_cast<ASizeT>(_mm_popcnt_u64(v));
  }

  const ASizeT bitCount(const Float_32 v) noexcept {
    return bitCount(_mm_set_ss(v));
  }

  const ASizeT bitCount(const Float_64 v) noexcept {
    return bitCount(_mm_set_sd(v));
  }

  const ASizeT bitCount(const PackedInts_128 &v) noexcept {
    return static_cast<ASizeT>(_mm_popcnt_si128(v));
  }

  const ASizeT bitCount(const PackedFloat2_128 &v) noexcept {
    return static_cast<ASizeT>(_mm_popcnt_pd(v));
  }

  const ASizeT bitCount(const PackedFloat4_128 &v) noexcept {
    return static_cast<ASizeT>(_mm_popcnt_ps(v));
  }

  const ASizeT bitCount(const PackedInts_256 &v) noexcept {
    return static_cast<ASizeT>(_mm256_popcnt_si256(v));
  }

  const ASizeT bitCount(const PackedFloat4_256 &v) noexcept {
    return static_cast<ASizeT>(_mm256_popcnt_pd(v));
  }

  const ASizeT bitCount(const PackedFloat8_256 &v) noexcept {
    return static_cast<ASizeT>(_mm256_popcnt_ps(v));
  }
}