#include "vectorization/v_ui64_4/operators.h"

#include "vectorization/v_ui64_4/accessors.h"
#include "vectorization/v_ui64_4/constants.h"

namespace vectorization {
  v_ui64_4 operator!(const v_ui64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_cmpeq_epi64(v.components, Zero<v_ui64_4::PackedType>());
#elif VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    const auto z = Zero<PackedInts_128>();
    const auto lo = _mm_cmpeq_epi64(_mm256_castsi256_si128(v.components), z);
    const auto hi = _mm_cmpeq_epi64(_mm256_extractf128_si256(v.components, 1), z);
    return _mm256_insertf128_si256(_mm256_castsi128_si256(lo), hi, 1);
#else
    const auto z = Zero<v_ui64_4::HalfPackedType>();
    return {{_mm_cmpeq_epi64(v.components.lo, z), _mm_cmpeq_epi64(v.components.hi, z)}};
#endif
  }

  v_ui64_4 operator~(const v_ui64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    const auto allOnes = _mm256_cmpeq_epi64(v.components, v.components);
    return _mm256_xor_si256(v.components, allOnes);
#elif VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    const auto lo = _mm256_castsi256_si128(v.components);
    const auto allOnes = _mm_cmpeq_epi64(lo, lo);
    const auto rlo = _mm_xor_si128(_mm256_castsi256_si128(v.components), allOnes);
    const auto rhi = _mm_xor_si128(_mm256_extractf128_si256(v.components, 1), allOnes);
    return _mm256_insertf128_si256(_mm256_castsi128_si256(rlo), rhi, 1);
#else
    const auto allOnes = _mm_cmpeq_epi64(v.components.lo, v.components.lo);
    return {{_mm_xor_si128(v.components.lo, allOnes), _mm_xor_si128(v.components.hi, allOnes)}};
#endif
  }

  v_ui64_4 operator+(const v_ui64_4 &a, const v_ui64_4 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_add_epi64(a.components, b.components);
#elif VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    const auto lo = _mm_add_epi64(_mm256_castsi256_si128(a.components), _mm256_castsi256_si128(b.components));
    const auto hi = _mm_add_epi64(_mm256_extractf128_si256(a.components, 1), _mm256_extractf128_si256(b.components, 1));
    return _mm256_insertf128_si256(_mm256_castsi128_si256(lo), hi, 1);
#else
    return {{_mm_add_epi64(a.components.lo, b.components.lo), _mm_add_epi64(a.components.hi, b.components.hi)}};
#endif
  }

  v_ui64_4 operator-(const v_ui64_4 &a, const v_ui64_4 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_sub_epi64(a.components, b.components);
#elif VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    const auto lo = _mm_sub_epi64(_mm256_castsi256_si128(a.components), _mm256_castsi256_si128(b.components));
    const auto hi = _mm_sub_epi64(_mm256_extractf128_si256(a.components, 1), _mm256_extractf128_si256(b.components, 1));
    return _mm256_insertf128_si256(_mm256_castsi128_si256(lo), hi, 1);
#else
    return {{_mm_sub_epi64(a.components.lo, b.components.lo), _mm_sub_epi64(a.components.hi, b.components.hi)}};
#endif
  }

  v_ui64_4 operator&(const v_ui64_4 &a, const v_ui64_4 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_and_si256(a.components, b.components);
#elif VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    const auto lo = _mm_and_si128(_mm256_castsi256_si128(a.components), _mm256_castsi256_si128(b.components));
    const auto hi = _mm_and_si128(_mm256_extractf128_si256(a.components, 1), _mm256_extractf128_si256(b.components, 1));
    return _mm256_insertf128_si256(_mm256_castsi128_si256(lo), hi, 1);
#else
    return {{_mm_and_si128(a.components.lo, b.components.lo), _mm_and_si128(a.components.hi, b.components.hi)}};
#endif
  }

  v_ui64_4 operator|(const v_ui64_4 &a, const v_ui64_4 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_or_si256(a.components, b.components);
#elif VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    const auto lo = _mm_or_si128(_mm256_castsi256_si128(a.components), _mm256_castsi256_si128(b.components));
    const auto hi = _mm_or_si128(_mm256_extractf128_si256(a.components, 1), _mm256_extractf128_si256(b.components, 1));
    return _mm256_insertf128_si256(_mm256_castsi128_si256(lo), hi, 1);
#else
    return {{_mm_or_si128(a.components.lo, b.components.lo), _mm_or_si128(a.components.hi, b.components.hi)}};
#endif
  }

  v_ui64_4 operator^(const v_ui64_4 &a, const v_ui64_4 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_xor_si256(a.components, b.components);
#elif VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    const auto lo = _mm_xor_si128(_mm256_castsi256_si128(a.components), _mm256_castsi256_si128(b.components));
    const auto hi = _mm_xor_si128(_mm256_extractf128_si256(a.components, 1), _mm256_extractf128_si256(b.components, 1));
    return _mm256_insertf128_si256(_mm256_castsi128_si256(lo), hi, 1);
#else
    return {{_mm_xor_si128(a.components.lo, b.components.lo), _mm_xor_si128(a.components.hi, b.components.hi)}};
#endif
  }

  v_ui64_4 operator>>(const v_ui64_4 &a, const v_ui64_4 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_srlv_epi64(a.components, b.components);
#else
    v_ui64_4 result;
    for (ASizeT i = 0; i < v_ui64_4::SIZE; ++i)
      result[i] = a[i] >> b[i];
    return result;
#endif
  }

  v_ui64_4 operator<(const v_ui64_4 &a, const v_ui64_4 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    const auto signBit = _mm256_set1_epi64x(static_cast<long long>(0x8000000000000000ULL));
    return _mm256_cmpgt_epi64(_mm256_xor_si256(b.components, signBit), _mm256_xor_si256(a.components, signBit));
#elif VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    const auto signBit = _mm_set1_epi64x(static_cast<long long>(0x8000000000000000ULL));
    const auto alo = _mm_xor_si128(_mm256_castsi256_si128(a.components), signBit);
    const auto ahi = _mm_xor_si128(_mm256_extractf128_si256(a.components, 1), signBit);
    const auto blo = _mm_xor_si128(_mm256_castsi256_si128(b.components), signBit);
    const auto bhi = _mm_xor_si128(_mm256_extractf128_si256(b.components, 1), signBit);
    const auto lo = _mm_cmpgt_epi64(blo, alo);
    const auto hi = _mm_cmpgt_epi64(bhi, ahi);
    return _mm256_insertf128_si256(_mm256_castsi128_si256(lo), hi, 1);
#else
    const auto signBit = _mm_set1_epi64x(static_cast<long long>(0x8000000000000000ULL));
    return {{_mm_cmpgt_epi64(_mm_xor_si128(b.components.lo, signBit), _mm_xor_si128(a.components.lo, signBit)),
             _mm_cmpgt_epi64(_mm_xor_si128(b.components.hi, signBit), _mm_xor_si128(a.components.hi, signBit))}};
#endif
  }

  v_ui64_4 operator>(const v_ui64_4 &a, const v_ui64_4 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    const auto signBit = _mm256_set1_epi64x(static_cast<long long>(0x8000000000000000ULL));
    return _mm256_cmpgt_epi64(_mm256_xor_si256(a.components, signBit), _mm256_xor_si256(b.components, signBit));
#elif VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    const auto signBit = _mm_set1_epi64x(static_cast<long long>(0x8000000000000000ULL));
    const auto alo = _mm_xor_si128(_mm256_castsi256_si128(a.components), signBit);
    const auto ahi = _mm_xor_si128(_mm256_extractf128_si256(a.components, 1), signBit);
    const auto blo = _mm_xor_si128(_mm256_castsi256_si128(b.components), signBit);
    const auto bhi = _mm_xor_si128(_mm256_extractf128_si256(b.components, 1), signBit);
    const auto lo = _mm_cmpgt_epi64(alo, blo);
    const auto hi = _mm_cmpgt_epi64(ahi, bhi);
    return _mm256_insertf128_si256(_mm256_castsi128_si256(lo), hi, 1);
#else
    const auto signBit = _mm_set1_epi64x(static_cast<long long>(0x8000000000000000ULL));
    return {{_mm_cmpgt_epi64(_mm_xor_si128(a.components.lo, signBit), _mm_xor_si128(b.components.lo, signBit)),
             _mm_cmpgt_epi64(_mm_xor_si128(a.components.hi, signBit), _mm_xor_si128(b.components.hi, signBit))}};
#endif
  }

  v_ui64_4 operator==(const v_ui64_4 &a, const v_ui64_4 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_cmpeq_epi64(a.components, b.components);
#elif VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    const auto lo = _mm_cmpeq_epi64(_mm256_castsi256_si128(a.components), _mm256_castsi256_si128(b.components));
    const auto hi = _mm_cmpeq_epi64(_mm256_extractf128_si256(a.components, 1), _mm256_extractf128_si256(b.components, 1));
    return _mm256_insertf128_si256(_mm256_castsi128_si256(lo), hi, 1);
#else
    return {{_mm_cmpeq_epi64(a.components.lo, b.components.lo), _mm_cmpeq_epi64(a.components.hi, b.components.hi)}};
#endif
  }

  v_ui64_4 operator!=(const v_ui64_4 &a, const v_ui64_4 &b) noexcept {
    return ~(a == b);
  }

  v_ui64_4 &operator+=(v_ui64_4 &a, const v_ui64_4 &b) noexcept { return a = a + b; }
  v_ui64_4 &operator-=(v_ui64_4 &a, const v_ui64_4 &b) noexcept { return a = a - b; }
  v_ui64_4 &operator&=(v_ui64_4 &a, const v_ui64_4 &b) noexcept { return a = a & b; }
  v_ui64_4 &operator|=(v_ui64_4 &a, const v_ui64_4 &b) noexcept { return a = a | b; }
  v_ui64_4 &operator^=(v_ui64_4 &a, const v_ui64_4 &b) noexcept { return a = a ^ b; }
  v_ui64_4 &operator>>=(v_ui64_4 &a, const v_ui64_4 &b) noexcept { return a = a >> b; }

  std::ostream &operator<<(std::ostream &stream, const v_ui64_4 &v) {
    return stream << "{" << x(v) << ", " << y(v) << ", " << z(v) << ", " << w(v) << "}";
  }
}
