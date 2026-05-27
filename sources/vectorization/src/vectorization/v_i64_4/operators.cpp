#include "vectorization/v_i64_4/operators.h"

#include "vectorization/v_i64_4/accessors.h"
#include "vectorization/v_i64_4/constants.h"
#include "vectorization/functions/bitwise.h"
#include "vectorization/functions/shift.h"
#include "vectorization/v_ui64_4/type.h"

namespace vectorization {
  v_i64_4 operator-(const v_i64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_sub_epi64(Zero<v_i64_4::PackedType>(), v.components);
#elif VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    const auto z = Zero<PackedInts_128>();
    const auto lo = _mm_sub_epi64(z, _mm256_castsi256_si128(v.components));
    const auto hi = _mm_sub_epi64(z, _mm256_extractf128_si256(v.components, 1));
    return _mm256_insertf128_si256(_mm256_castsi128_si256(lo), hi, 1);
#else
    const auto z = Zero<v_i64_4::HalfPackedType>();
    return {{_mm_sub_epi64(z, v.components.lo), _mm_sub_epi64(z, v.components.hi)}};
#endif
  }

  v_i64_4 operator!(const v_i64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_cmpeq_epi64(v.components, Zero<v_i64_4::PackedType>());
#elif VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    const auto z = Zero<PackedInts_128>();
    const auto lo = _mm_cmpeq_epi64(_mm256_castsi256_si128(v.components), z);
    const auto hi = _mm_cmpeq_epi64(_mm256_extractf128_si256(v.components, 1), z);
    return _mm256_insertf128_si256(_mm256_castsi128_si256(lo), hi, 1);
#else
    const auto z = Zero<v_i64_4::HalfPackedType>();
    return {{_mm_cmpeq_epi64(v.components.lo, z), _mm_cmpeq_epi64(v.components.hi, z)}};
#endif
  }

  v_i64_4 operator~(const v_i64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return bitwiseNot(v.components);
#else
    return {{bitwiseNot(v.components.lo), bitwiseNot(v.components.hi)}};
#endif
  }

  v_i64_4 operator+(const v_i64_4 &a, const v_i64_4 &b) noexcept {
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

  v_i64_4 operator-(const v_i64_4 &a, const v_i64_4 &b) noexcept {
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

  v_i64_4 operator&(const v_i64_4 &a, const v_i64_4 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return bitwiseAnd(a.components, b.components);
#else
    return {{bitwiseAnd(a.components.lo, b.components.lo), bitwiseAnd(a.components.hi, b.components.hi)}};
#endif
  }

  v_i64_4 operator|(const v_i64_4 &a, const v_i64_4 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return bitwiseOr(a.components, b.components);
#else
    return {{bitwiseOr(a.components.lo, b.components.lo), bitwiseOr(a.components.hi, b.components.hi)}};
#endif
  }

  v_i64_4 operator^(const v_i64_4 &a, const v_i64_4 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return bitwiseXor(a.components, b.components);
#else
    return {{bitwiseXor(a.components.lo, b.components.lo), bitwiseXor(a.components.hi, b.components.hi)}};
#endif
  }

  v_i64_4 operator<<(const v_i64_4 &a, const Int_64 count) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return shiftLeft64(a.components, count);
#else
    return {{shiftLeft64(a.components.lo, count), shiftLeft64(a.components.hi, count)}};
#endif
  }

  v_i64_4 operator<<(const v_i64_4 &a, const UInt_64 count) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return shiftLeft64(a.components, count);
#else
    return {{shiftLeft64(a.components.lo, count), shiftLeft64(a.components.hi, count)}};
#endif
  }

  v_i64_4 operator<<(const v_i64_4 &a, const v_i64_4 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return shiftLeft64(a.components, b.components);
#else
    return {{shiftLeft64(a.components.lo, b.components.lo), shiftLeft64(a.components.hi, b.components.hi)}};
#endif
  }

  v_i64_4 operator<<(const v_i64_4 &a, const v_ui64_4 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return shiftLeft64(a.components, b.components);
#else
    return {{shiftLeft64(a.components.lo, b.components.lo), shiftLeft64(a.components.hi, b.components.hi)}};
#endif
  }

  v_i64_4 operator>>(const v_i64_4 &a, const Int_64 count) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return shiftRightArithmetic64(a.components, count);
#else
    return {{shiftRightArithmetic64(a.components.lo, count), shiftRightArithmetic64(a.components.hi, count)}};
#endif
  }

  v_i64_4 operator>>(const v_i64_4 &a, const UInt_64 count) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return shiftRightArithmetic64(a.components, count);
#else
    return {{shiftRightArithmetic64(a.components.lo, count), shiftRightArithmetic64(a.components.hi, count)}};
#endif
  }

  v_i64_4 operator>>(const v_i64_4 &a, const v_i64_4 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return shiftRightArithmetic64(a.components, b.components);
#else
    return {{shiftRightArithmetic64(a.components.lo, b.components.lo),
             shiftRightArithmetic64(a.components.hi, b.components.hi)}};
#endif
  }

  v_i64_4 operator>>(const v_i64_4 &a, const v_ui64_4 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return shiftRightArithmetic64(a.components, b.components);
#else
    return {{shiftRightArithmetic64(a.components.lo, b.components.lo),
             shiftRightArithmetic64(a.components.hi, b.components.hi)}};
#endif
  }

  v_i64_4 operator==(const v_i64_4 &a, const v_i64_4 &b) noexcept {
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

  v_i64_4 operator!=(const v_i64_4 &a, const v_i64_4 &b) noexcept {
    return ~(a == b);
  }

  v_i64_4 operator<(const v_i64_4 &a, const v_i64_4 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_cmpgt_epi64(b.components, a.components);
#elif VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    const auto lo = _mm_cmpgt_epi64(_mm256_castsi256_si128(b.components), _mm256_castsi256_si128(a.components));
    const auto hi = _mm_cmpgt_epi64(_mm256_extractf128_si256(b.components, 1), _mm256_extractf128_si256(a.components, 1));
    return _mm256_insertf128_si256(_mm256_castsi128_si256(lo), hi, 1);
#else
    return {{_mm_cmpgt_epi64(b.components.lo, a.components.lo), _mm_cmpgt_epi64(b.components.hi, a.components.hi)}};
#endif
  }

  v_i64_4 operator>(const v_i64_4 &a, const v_i64_4 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_cmpgt_epi64(a.components, b.components);
#elif VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    const auto lo = _mm_cmpgt_epi64(_mm256_castsi256_si128(a.components), _mm256_castsi256_si128(b.components));
    const auto hi = _mm_cmpgt_epi64(_mm256_extractf128_si256(a.components, 1), _mm256_extractf128_si256(b.components, 1));
    return _mm256_insertf128_si256(_mm256_castsi128_si256(lo), hi, 1);
#else
    return {{_mm_cmpgt_epi64(a.components.lo, b.components.lo), _mm_cmpgt_epi64(a.components.hi, b.components.hi)}};
#endif
  }

  v_i64_4 &operator+=(v_i64_4 &a, const v_i64_4 &b) noexcept { return a = a + b; }
  v_i64_4 &operator-=(v_i64_4 &a, const v_i64_4 &b) noexcept { return a = a - b; }
  v_i64_4 &operator&=(v_i64_4 &a, const v_i64_4 &b) noexcept { return a = a & b; }
  v_i64_4 &operator|=(v_i64_4 &a, const v_i64_4 &b) noexcept { return a = a | b; }
  v_i64_4 &operator^=(v_i64_4 &a, const v_i64_4 &b) noexcept { return a = a ^ b; }

  v_i64_4 &operator<<=(v_i64_4 &a, const Int_64 count) noexcept { return a = a << count; }
  v_i64_4 &operator<<=(v_i64_4 &a, const UInt_64 count) noexcept { return a = a << count; }
  v_i64_4 &operator<<=(v_i64_4 &a, const v_i64_4 &b) noexcept { return a = a << b; }
  v_i64_4 &operator<<=(v_i64_4 &a, const v_ui64_4 &b) noexcept { return a = a << b; }

  v_i64_4 &operator>>=(v_i64_4 &a, const Int_64 count) noexcept { return a = a >> count; }
  v_i64_4 &operator>>=(v_i64_4 &a, const UInt_64 count) noexcept { return a = a >> count; }
  v_i64_4 &operator>>=(v_i64_4 &a, const v_i64_4 &b) noexcept { return a = a >> b; }
  v_i64_4 &operator>>=(v_i64_4 &a, const v_ui64_4 &b) noexcept { return a = a >> b; }

  std::ostream &operator<<(std::ostream &stream, const v_i64_4 &v) {
    return stream << "{" << x(v) << ", " << y(v) << ", " << z(v) << ", " << w(v) << "}";
  }
}