#include "vectorization/v_ui64_4/operators.h"

#include "vectorization/v_ui64_4/accessors.h"
#include "vectorization/v_ui64_4/constants.h"
#include "vectorization/functions/bitwise.h"
#include "vectorization/functions/shift.h"
#include "vectorization/v_i64_4/type.h"

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
    return bitwiseNot(v.components);
#else
    return {{bitwiseNot(v.components.lo), bitwiseNot(v.components.hi)}};
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
    return bitwiseAnd(a.components, b.components);
#else
    return {{bitwiseAnd(a.components.lo, b.components.lo), bitwiseAnd(a.components.hi, b.components.hi)}};
#endif
  }

  v_ui64_4 operator|(const v_ui64_4 &a, const v_ui64_4 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return bitwiseOr(a.components, b.components);
#else
    return {{bitwiseOr(a.components.lo, b.components.lo), bitwiseOr(a.components.hi, b.components.hi)}};
#endif
  }

  v_ui64_4 operator^(const v_ui64_4 &a, const v_ui64_4 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return bitwiseXor(a.components, b.components);
#else
    return {{bitwiseXor(a.components.lo, b.components.lo), bitwiseXor(a.components.hi, b.components.hi)}};
#endif
  }

  v_ui64_4 operator<<(const v_ui64_4 &a, const Int_64 count) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return shiftLeft64(a.components, count);
#else
    return {{shiftLeft64(a.components.lo, count), shiftLeft64(a.components.hi, count)}};
#endif
  }

  v_ui64_4 operator<<(const v_ui64_4 &a, const UInt_64 count) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return shiftLeft64(a.components, count);
#else
    return {{shiftLeft64(a.components.lo, count), shiftLeft64(a.components.hi, count)}};
#endif
  }

  v_ui64_4 operator<<(const v_ui64_4 &a, const v_i64_4 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return shiftLeft64(a.components, b.components);
#else
    return {{shiftLeft64(a.components.lo, b.components.lo), shiftLeft64(a.components.hi, b.components.hi)}};
#endif
  }

  v_ui64_4 operator<<(const v_ui64_4 &a, const v_ui64_4 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return shiftLeft64(a.components, b.components);
#else
    return {{shiftLeft64(a.components.lo, b.components.lo), shiftLeft64(a.components.hi, b.components.hi)}};
#endif
  }

  v_ui64_4 operator>>(const v_ui64_4 &a, const Int_64 count) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return shiftRightLogical64(a.components, count);
#else
    return {{shiftRightLogical64(a.components.lo, count), shiftRightLogical64(a.components.hi, count)}};
#endif
  }

  v_ui64_4 operator>>(const v_ui64_4 &a, const UInt_64 count) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return shiftRightLogical64(a.components, count);
#else
    return {{shiftRightLogical64(a.components.lo, count), shiftRightLogical64(a.components.hi, count)}};
#endif
  }

  v_ui64_4 operator>>(const v_ui64_4 &a, const v_i64_4 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return shiftRightLogical64(a.components, b.components);
#else
    return {{shiftRightLogical64(a.components.lo, b.components.lo),
             shiftRightLogical64(a.components.hi, b.components.hi)}};
#endif
  }

  v_ui64_4 operator>>(const v_ui64_4 &a, const v_ui64_4 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return shiftRightLogical64(a.components, b.components);
#else
    return {{shiftRightLogical64(a.components.lo, b.components.lo),
             shiftRightLogical64(a.components.hi, b.components.hi)}};
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

  v_ui64_4 &operator<<=(v_ui64_4 &a, const Int_64 count) noexcept { return a = a << count; }
  v_ui64_4 &operator<<=(v_ui64_4 &a, const UInt_64 count) noexcept { return a = a << count; }
  v_ui64_4 &operator<<=(v_ui64_4 &a, const v_i64_4 &b) noexcept { return a = a << b; }
  v_ui64_4 &operator<<=(v_ui64_4 &a, const v_ui64_4 &b) noexcept { return a = a << b; }

  v_ui64_4 &operator>>=(v_ui64_4 &a, const Int_64 count) noexcept { return a = a >> count; }
  v_ui64_4 &operator>>=(v_ui64_4 &a, const UInt_64 count) noexcept { return a = a >> count; }
  v_ui64_4 &operator>>=(v_ui64_4 &a, const v_i64_4 &b) noexcept { return a = a >> b; }
  v_ui64_4 &operator>>=(v_ui64_4 &a, const v_ui64_4 &b) noexcept { return a = a >> b; }

  std::ostream &operator<<(std::ostream &stream, const v_ui64_4 &v) {
    return stream << "{" << x(v) << ", " << y(v) << ", " << z(v) << ", " << w(v) << "}";
  }
}
