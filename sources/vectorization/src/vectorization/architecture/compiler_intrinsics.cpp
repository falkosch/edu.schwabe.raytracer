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
    // see https://stackoverflow.com/a/34067907
    // v2 = v / 2
    const auto half = _mm_srli_epi32(values, 1);
    // ret = v - (v / 2)
    return _mm_add_ps(_mm_cvtepi32_ps(half), _mm_cvtepi32_ps(_mm_sub_epi32(values, half)));
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
