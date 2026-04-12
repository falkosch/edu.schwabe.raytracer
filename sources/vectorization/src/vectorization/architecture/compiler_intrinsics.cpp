#include "vectorization/architecture/compiler_intrinsics.h"

#ifndef ARCH_X64
#include <array>
#endif

#include <cstring>

namespace vectorization {
  PackedInts_128 _mm_load_si128(const UInt_32 *const values) noexcept {
    PackedInts_128 target;
    std::memcpy(&target, values, sizeof target);
    return target;
  }

  PackedFloat4_128 _mm_cvtepu32_ps(const PackedInts_128 &values) noexcept {
    // Generate sign bit mask (0x80000000) without literals: all-ones shifted left by 31.
    const auto allOnes = _mm_cmpeq_epi32(_mm_setzero_si128(), _mm_setzero_si128());
    const auto highBit = _mm_slli_epi32(allOnes, 31);
    // Clear the high bit so the lower 31 bits fit in signed int32 range for exact conversion.
    const auto low31 = _mm_andnot_si128(highBit, values);
    // Derive 2^31 as float: convert signed 0x80000000 (-2147483648) then strip sign bit.
    const auto highBitFloat = _mm_andnot_ps(_mm_castsi128_ps(highBit), _mm_cvtepi32_ps(highBit));
    // Build a mask for elements that had the high bit set.
    const auto hadHighBit = _mm_castsi128_ps(_mm_cmpeq_epi32(_mm_and_si128(values, highBit), highBit));
    // Convert lower 31 bits and conditionally add 2^31 for elements that had it set.
    return _mm_add_ps(_mm_cvtepi32_ps(low31), _mm_and_ps(hadHighBit, highBitFloat));
  }

  Int_64 _mm_popcnt_u64(const UInt_64 value) noexcept {
    // see https://stackoverflow.com/a/17355341
#ifdef ARCH_X64
    return ::_mm_popcnt_u64(value);
#else
    const auto count =
        _mm_popcnt_u32(static_cast<UInt_32>(value & 0xffffffff)) + _mm_popcnt_u32(static_cast<UInt_32>(value >> 32));
    return Int_64{count};
#endif
  }

  inline UInt_64 x_64(const PackedInts_128 &values) noexcept {
#ifdef ARCH_X64
    return static_cast<UInt_64>(_mm_cvtsi128_si64(values));
#else
    std::array<UInt_64, 2> bits;
    std::memcpy(bits.data(), &values, sizeof values);
    return bits.front();
#endif
  }

  Int_64 _mm_popcnt_si128(const PackedInts_128 &values) noexcept {
    // see https://stackoverflow.com/a/17355341
    return _mm_popcnt_u64(x_64(values)) + _mm_popcnt_u64(x_64(_mm_unpackhi_epi64(values, values)));
  }

  Int_64 _mm256_popcnt_si256(const PackedInts_256 &values) noexcept {
    return _mm_popcnt_si128(_mm256_castsi256_si128(values)) + _mm_popcnt_si128(_mm256_extractf128_si256(values, 1));
  }

  Int_64 _mm_popcnt_pd(const PackedFloat2_128 &values) noexcept {
    return _mm_popcnt_si128(_mm_castpd_si128(values));
  }

  Int_64 _mm_popcnt_ps(const PackedFloat4_128 &values) noexcept {
    return _mm_popcnt_si128(_mm_castps_si128(values));
  }

  Int_64 _mm256_popcnt_ps(const PackedFloat8_256 &values) noexcept {
    return _mm256_popcnt_si256(_mm256_castps_si256(values));
  }

  Int_64 _mm256_popcnt_pd(const PackedFloat4_256 &values) noexcept {
    return _mm256_popcnt_si256(_mm256_castpd_si256(values));
  }
}
