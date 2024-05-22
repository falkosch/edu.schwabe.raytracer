/*
 * Includes the required header for the intrinsics defines some useful macros not always readily available for all
 * compilers.
 */

#pragma once

#include "compiler_definitions.h"

#if defined(__GNUC__)
#include <x86intrin.h>
#else
#include <intrin.h>
#endif

#include <cstddef>
#include <cstdint>

#define BLEND_MASK_INT4_32(X, Y, Z, W)                                                                 \
  (((X & 1) << 0) | ((X & 1) << 1) | ((Y & 1) << 2) | ((Y & 1) << 3) | ((Z & 1) << 4) | ((Z & 1) << 5) \
   | ((W & 1) << 6) | ((W & 1) << 7))

// GCC doesn't know avxintrin.h macros for some reason
#ifndef _mm256_set_m128
#define _mm256_set_m128(/* __m128 */ hi, /* __m128 */ lo) _mm256_insertf128_ps(_mm256_castps128_ps256(lo), (hi), 0x1)
#endif
#ifndef _mm256_set_m128d
#define _mm256_set_m128d(/* __m128d */ hi, /* __m128d */ lo) _mm256_insertf128_pd(_mm256_castpd128_pd256(lo), (hi), 0x1)
#endif
#ifndef _mm256_set_m128i
#define _mm256_set_m128i(/* __m128i */ hi, /* __m128i */ lo) \
  _mm256_insertf128_si256(_mm256_castsi128_si256(lo), (hi), 0x1)
#endif

namespace vectorization {
  using ASizeT = std::size_t;

  using Int_8 = int8_t;
  using Int_16 = int16_t;
  using Int_32 = int32_t;
  using Int_64 = int64_t;

  using UInt_8 = uint8_t;
  using UInt_16 = uint16_t;
  using UInt_32 = uint32_t;
  using UInt_64 = uint64_t;

  using Float_32 = float;
  using Float_64 = double;

  using Bool_8 = Int_8;
  using Bool_16 = Int_16;
  using Bool_32 = Int_32;
  using Bool_64 = Int_64;

  using PackedFloat4_128 = __m128;
  using PackedFloat2_128 = __m128d;
  using PackedInts_128 = __m128i;

  using PackedFloat8_256 = __m256;
  using PackedFloat4_256 = __m256d;
  using PackedInts_256 = __m256i;

  PackedInts_128 _mm_load_si128(const UInt_32 *values) noexcept;

  /**
   * @brief converts unsigned ints to floats. _mm_cvtepu32_ps() is first supported in AVX512, so that this function is
   * an alternative SSE2-based impl.
   * @param values unsigned ints to convert
   * @return float representation of v
   */
  PackedFloat4_128 _mm_cvtepu32_ps(const PackedInts_128 &values) noexcept;

  Int_64 _mm_popcnt_u64(UInt_64 value) noexcept;

  Int_64 _mm_popcnt_si128(const PackedInts_128 &values) noexcept;
  Int_64 _mm_popcnt_ps(const PackedFloat4_128 &values) noexcept;
  Int_64 _mm_popcnt_pd(const PackedFloat2_128 &values) noexcept;

  Int_64 _mm256_popcnt_si256(const PackedInts_256 &values) noexcept;
  Int_64 _mm256_popcnt_ps(const PackedFloat8_256 &values) noexcept;
  Int_64 _mm256_popcnt_pd(const PackedFloat4_256 &values) noexcept;
}
