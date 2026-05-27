#include "vectorization/functions/shift.h"

namespace vectorization {
  // -- shiftLeft32 per-lane --

  PackedInts_128 shiftLeft32(const PackedInts_128 &a, const PackedInts_128 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm_sllv_epi32(a, b);
#else
    alignas(XMM_ALIGNMENT) UInt_32 va[4], vb[4];
    _mm_store_si128(reinterpret_cast<__m128i *>(va), a);
    _mm_store_si128(reinterpret_cast<__m128i *>(vb), b);
    for (int i = 0; i < 4; ++i)
      va[i] = (va[i] << (vb[i] & 31u)) & static_cast<UInt_32>(-static_cast<Int_32>(vb[i] < 32u));
    return _mm_load_si128(reinterpret_cast<const __m128i *>(va));
#endif
  }

  PackedInts_256 shiftLeft32(const PackedInts_256 &a, const PackedInts_256 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_sllv_epi32(a, b);
#else
    return _mm256_set_m128i(shiftLeft32(_mm256_extractf128_si256(a, 1), _mm256_extractf128_si256(b, 1)),
                            shiftLeft32(_mm256_castsi256_si128(a), _mm256_castsi256_si128(b)));
#endif
  }

  // -- shiftLeft32 scalar uniform --

  PackedInts_128 shiftLeft32(const PackedInts_128 &a, const Int_32 count) noexcept {
    return _mm_sll_epi32(a, _mm_cvtsi32_si128(count));
  }

  PackedInts_128 shiftLeft32(const PackedInts_128 &a, const UInt_32 count) noexcept {
    return shiftLeft32(a, static_cast<Int_32>(count));
  }

  PackedInts_256 shiftLeft32(const PackedInts_256 &a, const Int_32 count) noexcept {
    const auto shift = _mm_cvtsi32_si128(count);
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_sll_epi32(a, shift);
#else
    return _mm256_set_m128i(_mm_sll_epi32(_mm256_extractf128_si256(a, 1), shift),
                            _mm_sll_epi32(_mm256_castsi256_si128(a), shift));
#endif
  }

  PackedInts_256 shiftLeft32(const PackedInts_256 &a, const UInt_32 count) noexcept {
    return shiftLeft32(a, static_cast<Int_32>(count));
  }

  // -- shiftRightLogical32 per-lane --

  PackedInts_128 shiftRightLogical32(const PackedInts_128 &a, const PackedInts_128 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm_srlv_epi32(a, b);
#else
    alignas(XMM_ALIGNMENT) UInt_32 va[4], vb[4];
    _mm_store_si128(reinterpret_cast<__m128i *>(va), a);
    _mm_store_si128(reinterpret_cast<__m128i *>(vb), b);
    for (int i = 0; i < 4; ++i)
      va[i] = (va[i] >> (vb[i] & 31u)) & static_cast<UInt_32>(-static_cast<Int_32>(vb[i] < 32u));
    return _mm_load_si128(reinterpret_cast<const __m128i *>(va));
#endif
  }

  PackedInts_256 shiftRightLogical32(const PackedInts_256 &a, const PackedInts_256 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_srlv_epi32(a, b);
#else
    return _mm256_set_m128i(
        shiftRightLogical32(_mm256_extractf128_si256(a, 1), _mm256_extractf128_si256(b, 1)),
        shiftRightLogical32(_mm256_castsi256_si128(a), _mm256_castsi256_si128(b)));
#endif
  }

  // -- shiftRightLogical32 scalar uniform --

  PackedInts_128 shiftRightLogical32(const PackedInts_128 &a, const Int_32 count) noexcept {
    return _mm_srl_epi32(a, _mm_cvtsi32_si128(count));
  }

  PackedInts_128 shiftRightLogical32(const PackedInts_128 &a, const UInt_32 count) noexcept {
    return shiftRightLogical32(a, static_cast<Int_32>(count));
  }

  PackedInts_256 shiftRightLogical32(const PackedInts_256 &a, const Int_32 count) noexcept {
    const auto shift = _mm_cvtsi32_si128(count);
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_srl_epi32(a, shift);
#else
    return _mm256_set_m128i(_mm_srl_epi32(_mm256_extractf128_si256(a, 1), shift),
                            _mm_srl_epi32(_mm256_castsi256_si128(a), shift));
#endif
  }

  PackedInts_256 shiftRightLogical32(const PackedInts_256 &a, const UInt_32 count) noexcept {
    return shiftRightLogical32(a, static_cast<Int_32>(count));
  }

  // -- shiftRightArithmetic32 per-lane --

  PackedInts_128 shiftRightArithmetic32(const PackedInts_128 &a, const PackedInts_128 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm_srav_epi32(a, b);
#else
    alignas(XMM_ALIGNMENT) Int_32 va[4];
    alignas(XMM_ALIGNMENT) UInt_32 vb[4];
    _mm_store_si128(reinterpret_cast<__m128i *>(va), a);
    _mm_store_si128(reinterpret_cast<__m128i *>(vb), b);
    for (int i = 0; i < 4; ++i)
      va[i] = va[i] >> (vb[i] < 32u ? vb[i] : 31u);
    return _mm_load_si128(reinterpret_cast<const __m128i *>(va));
#endif
  }

  PackedInts_256 shiftRightArithmetic32(const PackedInts_256 &a, const PackedInts_256 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_srav_epi32(a, b);
#else
    return _mm256_set_m128i(
        shiftRightArithmetic32(_mm256_extractf128_si256(a, 1), _mm256_extractf128_si256(b, 1)),
        shiftRightArithmetic32(_mm256_castsi256_si128(a), _mm256_castsi256_si128(b)));
#endif
  }

  // -- shiftRightArithmetic32 scalar uniform --

  PackedInts_128 shiftRightArithmetic32(const PackedInts_128 &a, const Int_32 count) noexcept {
    return _mm_sra_epi32(a, _mm_cvtsi32_si128(count));
  }

  PackedInts_128 shiftRightArithmetic32(const PackedInts_128 &a, const UInt_32 count) noexcept {
    return shiftRightArithmetic32(a, static_cast<Int_32>(count));
  }

  PackedInts_256 shiftRightArithmetic32(const PackedInts_256 &a, const Int_32 count) noexcept {
    const auto shift = _mm_cvtsi32_si128(count);
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_sra_epi32(a, shift);
#else
    return _mm256_set_m128i(_mm_sra_epi32(_mm256_extractf128_si256(a, 1), shift),
                            _mm_sra_epi32(_mm256_castsi256_si128(a), shift));
#endif
  }

  PackedInts_256 shiftRightArithmetic32(const PackedInts_256 &a, const UInt_32 count) noexcept {
    return shiftRightArithmetic32(a, static_cast<Int_32>(count));
  }

  // -- shiftLeft64 per-lane --

  PackedInts_128 shiftLeft64(const PackedInts_128 &a, const PackedInts_128 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm_sllv_epi64(a, b);
#else
    alignas(XMM_ALIGNMENT) UInt_64 va[2], vb[2];
    _mm_store_si128(reinterpret_cast<__m128i *>(va), a);
    _mm_store_si128(reinterpret_cast<__m128i *>(vb), b);
    va[0] = (va[0] << (vb[0] & 63u)) & static_cast<UInt_64>(-static_cast<Int_64>(vb[0] < 64u));
    va[1] = (va[1] << (vb[1] & 63u)) & static_cast<UInt_64>(-static_cast<Int_64>(vb[1] < 64u));
    return _mm_load_si128(reinterpret_cast<const __m128i *>(va));
#endif
  }

  PackedInts_256 shiftLeft64(const PackedInts_256 &a, const PackedInts_256 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_sllv_epi64(a, b);
#else
    return _mm256_set_m128i(shiftLeft64(_mm256_extractf128_si256(a, 1), _mm256_extractf128_si256(b, 1)),
                            shiftLeft64(_mm256_castsi256_si128(a), _mm256_castsi256_si128(b)));
#endif
  }

  // -- shiftLeft64 scalar uniform --

  PackedInts_128 shiftLeft64(const PackedInts_128 &a, const Int_64 count) noexcept {
    return _mm_sll_epi64(a, _mm_cvtsi64_si128(count));
  }

  PackedInts_128 shiftLeft64(const PackedInts_128 &a, const UInt_64 count) noexcept {
    return shiftLeft64(a, static_cast<Int_64>(count));
  }

  PackedInts_256 shiftLeft64(const PackedInts_256 &a, const Int_64 count) noexcept {
    const auto shift = _mm_cvtsi64_si128(count);
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_sll_epi64(a, shift);
#else
    return _mm256_set_m128i(_mm_sll_epi64(_mm256_extractf128_si256(a, 1), shift),
                            _mm_sll_epi64(_mm256_castsi256_si128(a), shift));
#endif
  }

  PackedInts_256 shiftLeft64(const PackedInts_256 &a, const UInt_64 count) noexcept {
    return shiftLeft64(a, static_cast<Int_64>(count));
  }

  // -- shiftRightLogical64 per-lane --

  PackedInts_128 shiftRightLogical64(const PackedInts_128 &a, const PackedInts_128 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm_srlv_epi64(a, b);
#else
    alignas(XMM_ALIGNMENT) UInt_64 va[2], vb[2];
    _mm_store_si128(reinterpret_cast<__m128i *>(va), a);
    _mm_store_si128(reinterpret_cast<__m128i *>(vb), b);
    va[0] = (va[0] >> (vb[0] & 63u)) & static_cast<UInt_64>(-static_cast<Int_64>(vb[0] < 64u));
    va[1] = (va[1] >> (vb[1] & 63u)) & static_cast<UInt_64>(-static_cast<Int_64>(vb[1] < 64u));
    return _mm_load_si128(reinterpret_cast<const __m128i *>(va));
#endif
  }

  PackedInts_256 shiftRightLogical64(const PackedInts_256 &a, const PackedInts_256 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_srlv_epi64(a, b);
#else
    return _mm256_set_m128i(
        shiftRightLogical64(_mm256_extractf128_si256(a, 1), _mm256_extractf128_si256(b, 1)),
        shiftRightLogical64(_mm256_castsi256_si128(a), _mm256_castsi256_si128(b)));
#endif
  }

  // -- shiftRightLogical64 scalar uniform --

  PackedInts_128 shiftRightLogical64(const PackedInts_128 &a, const Int_64 count) noexcept {
    return _mm_srl_epi64(a, _mm_cvtsi64_si128(count));
  }

  PackedInts_128 shiftRightLogical64(const PackedInts_128 &a, const UInt_64 count) noexcept {
    return shiftRightLogical64(a, static_cast<Int_64>(count));
  }

  PackedInts_256 shiftRightLogical64(const PackedInts_256 &a, const Int_64 count) noexcept {
    const auto shift = _mm_cvtsi64_si128(count);
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_srl_epi64(a, shift);
#else
    return _mm256_set_m128i(_mm_srl_epi64(_mm256_extractf128_si256(a, 1), shift),
                            _mm_srl_epi64(_mm256_castsi256_si128(a), shift));
#endif
  }

  PackedInts_256 shiftRightLogical64(const PackedInts_256 &a, const UInt_64 count) noexcept {
    return shiftRightLogical64(a, static_cast<Int_64>(count));
  }

  // -- shiftRightArithmetic64 per-lane (no intrinsic before AVX-512VL) --

  PackedInts_128 shiftRightArithmetic64(const PackedInts_128 &a, const PackedInts_128 &b) noexcept {
    alignas(XMM_ALIGNMENT) Int_64 va[2];
    alignas(XMM_ALIGNMENT) UInt_64 vb[2];
    _mm_store_si128(reinterpret_cast<__m128i *>(va), a);
    _mm_store_si128(reinterpret_cast<__m128i *>(vb), b);
    va[0] = va[0] >> (vb[0] < 64u ? vb[0] : 63u);
    va[1] = va[1] >> (vb[1] < 64u ? vb[1] : 63u);
    return _mm_load_si128(reinterpret_cast<const __m128i *>(va));
  }

  PackedInts_256 shiftRightArithmetic64(const PackedInts_256 &a, const PackedInts_256 &b) noexcept {
    return _mm256_set_m128i(
        shiftRightArithmetic64(_mm256_extractf128_si256(a, 1), _mm256_extractf128_si256(b, 1)),
        shiftRightArithmetic64(_mm256_castsi256_si128(a), _mm256_castsi256_si128(b)));
  }

  // -- shiftRightArithmetic64 scalar uniform (no intrinsic before AVX-512VL) --

  PackedInts_128 shiftRightArithmetic64(const PackedInts_128 &a, const Int_64 count) noexcept {
    alignas(XMM_ALIGNMENT) Int_64 va[2];
    _mm_store_si128(reinterpret_cast<__m128i *>(va), a);
    const auto clamped = static_cast<UInt_64>(count) < 64u ? count : Int_64{63};
    va[0] = va[0] >> clamped;
    va[1] = va[1] >> clamped;
    return _mm_load_si128(reinterpret_cast<const __m128i *>(va));
  }

  PackedInts_128 shiftRightArithmetic64(const PackedInts_128 &a, const UInt_64 count) noexcept {
    return shiftRightArithmetic64(a, static_cast<Int_64>(count));
  }

  PackedInts_256 shiftRightArithmetic64(const PackedInts_256 &a, const Int_64 count) noexcept {
    return _mm256_set_m128i(shiftRightArithmetic64(_mm256_extractf128_si256(a, 1), count),
                            shiftRightArithmetic64(_mm256_castsi256_si128(a), count));
  }

  PackedInts_256 shiftRightArithmetic64(const PackedInts_256 &a, const UInt_64 count) noexcept {
    return shiftRightArithmetic64(a, static_cast<Int_64>(count));
  }
}
