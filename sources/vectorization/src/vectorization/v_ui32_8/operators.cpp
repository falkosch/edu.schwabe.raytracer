#include "vectorization/v_ui32_8/operators.h"

#include "vectorization/v_ui32_8/accessors.h"
#include "vectorization/v_ui32_8/constants.h"
#include "vectorization/functions/bitwise.h"
#include "vectorization/functions/shift.h"
#include "vectorization/v_i32_8/type.h"

namespace vectorization {
  v_ui32_8 operator!(const v_ui32_8 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_cmpeq_epi32(v.components, Zero<v_ui32_8::PackedType>());
#elif VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    const auto z = Zero<PackedInts_128>();
    const auto lo = _mm_cmpeq_epi32(_mm256_castsi256_si128(v.components), z);
    const auto hi = _mm_cmpeq_epi32(_mm256_extractf128_si256(v.components, 1), z);
    return _mm256_insertf128_si256(_mm256_castsi128_si256(lo), hi, 1);
#else
    const auto z = Zero<v_ui32_8::HalfPackedType>();
    return {{_mm_cmpeq_epi32(v.components.lo, z), _mm_cmpeq_epi32(v.components.hi, z)}};
#endif
  }

  v_ui32_8 operator~(const v_ui32_8 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return bitwiseNot(v.components);
#else
    return {{bitwiseNot(v.components.lo), bitwiseNot(v.components.hi)}};
#endif
  }

  v_ui32_8 operator+(const v_ui32_8 &a, const v_ui32_8 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_add_epi32(a.components, b.components);
#elif VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    const auto lo = _mm_add_epi32(_mm256_castsi256_si128(a.components), _mm256_castsi256_si128(b.components));
    const auto hi = _mm_add_epi32(_mm256_extractf128_si256(a.components, 1), _mm256_extractf128_si256(b.components, 1));
    return _mm256_insertf128_si256(_mm256_castsi128_si256(lo), hi, 1);
#else
    return {{_mm_add_epi32(a.components.lo, b.components.lo), _mm_add_epi32(a.components.hi, b.components.hi)}};
#endif
  }

  v_ui32_8 operator-(const v_ui32_8 &a, const v_ui32_8 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_sub_epi32(a.components, b.components);
#elif VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    const auto lo = _mm_sub_epi32(_mm256_castsi256_si128(a.components), _mm256_castsi256_si128(b.components));
    const auto hi = _mm_sub_epi32(_mm256_extractf128_si256(a.components, 1), _mm256_extractf128_si256(b.components, 1));
    return _mm256_insertf128_si256(_mm256_castsi128_si256(lo), hi, 1);
#else
    return {{_mm_sub_epi32(a.components.lo, b.components.lo), _mm_sub_epi32(a.components.hi, b.components.hi)}};
#endif
  }

  v_ui32_8 operator*(const v_ui32_8 &a, const v_ui32_8 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_mullo_epi32(a.components, b.components);
#elif VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    const auto lo = _mm_mullo_epi32(_mm256_castsi256_si128(a.components), _mm256_castsi256_si128(b.components));
    const auto hi = _mm_mullo_epi32(_mm256_extractf128_si256(a.components, 1), _mm256_extractf128_si256(b.components, 1));
    return _mm256_insertf128_si256(_mm256_castsi128_si256(lo), hi, 1);
#else
    return {{_mm_mullo_epi32(a.components.lo, b.components.lo), _mm_mullo_epi32(a.components.hi, b.components.hi)}};
#endif
  }

  v_ui32_8 operator&(const v_ui32_8 &a, const v_ui32_8 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return bitwiseAnd(a.components, b.components);
#else
    return {{bitwiseAnd(a.components.lo, b.components.lo), bitwiseAnd(a.components.hi, b.components.hi)}};
#endif
  }

  v_ui32_8 operator|(const v_ui32_8 &a, const v_ui32_8 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return bitwiseOr(a.components, b.components);
#else
    return {{bitwiseOr(a.components.lo, b.components.lo), bitwiseOr(a.components.hi, b.components.hi)}};
#endif
  }

  v_ui32_8 operator^(const v_ui32_8 &a, const v_ui32_8 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return bitwiseXor(a.components, b.components);
#else
    return {{bitwiseXor(a.components.lo, b.components.lo), bitwiseXor(a.components.hi, b.components.hi)}};
#endif
  }

  v_ui32_8 operator<<(const v_ui32_8 &a, const Int_32 count) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return shiftLeft32(a.components, count);
#else
    return {{shiftLeft32(a.components.lo, count), shiftLeft32(a.components.hi, count)}};
#endif
  }

  v_ui32_8 operator<<(const v_ui32_8 &a, const UInt_32 count) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return shiftLeft32(a.components, count);
#else
    return {{shiftLeft32(a.components.lo, count), shiftLeft32(a.components.hi, count)}};
#endif
  }

  v_ui32_8 operator<<(const v_ui32_8 &a, const v_i32_8 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return shiftLeft32(a.components, b.components);
#else
    return {{shiftLeft32(a.components.lo, b.components.lo), shiftLeft32(a.components.hi, b.components.hi)}};
#endif
  }

  v_ui32_8 operator<<(const v_ui32_8 &a, const v_ui32_8 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return shiftLeft32(a.components, b.components);
#else
    return {{shiftLeft32(a.components.lo, b.components.lo), shiftLeft32(a.components.hi, b.components.hi)}};
#endif
  }

  v_ui32_8 operator>>(const v_ui32_8 &a, const Int_32 count) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return shiftRightLogical32(a.components, count);
#else
    return {{shiftRightLogical32(a.components.lo, count), shiftRightLogical32(a.components.hi, count)}};
#endif
  }

  v_ui32_8 operator>>(const v_ui32_8 &a, const UInt_32 count) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return shiftRightLogical32(a.components, count);
#else
    return {{shiftRightLogical32(a.components.lo, count), shiftRightLogical32(a.components.hi, count)}};
#endif
  }

  v_ui32_8 operator>>(const v_ui32_8 &a, const v_i32_8 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return shiftRightLogical32(a.components, b.components);
#else
    return {{shiftRightLogical32(a.components.lo, b.components.lo), shiftRightLogical32(a.components.hi, b.components.hi)}};
#endif
  }

  v_ui32_8 operator>>(const v_ui32_8 &a, const v_ui32_8 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return shiftRightLogical32(a.components, b.components);
#else
    return {{shiftRightLogical32(a.components.lo, b.components.lo), shiftRightLogical32(a.components.hi, b.components.hi)}};
#endif
  }

  v_ui32_8 operator<=(const v_ui32_8 &a, const v_ui32_8 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_cmpeq_epi32(a.components, _mm256_min_epu32(a.components, b.components));
#elif VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    const auto alo = _mm256_castsi256_si128(a.components);
    const auto ahi = _mm256_extractf128_si256(a.components, 1);
    const auto blo = _mm256_castsi256_si128(b.components);
    const auto bhi = _mm256_extractf128_si256(b.components, 1);
    const auto lo = _mm_cmpeq_epi32(alo, _mm_min_epu32(alo, blo));
    const auto hi = _mm_cmpeq_epi32(ahi, _mm_min_epu32(ahi, bhi));
    return _mm256_insertf128_si256(_mm256_castsi128_si256(lo), hi, 1);
#else
    return {{_mm_cmpeq_epi32(a.components.lo, _mm_min_epu32(a.components.lo, b.components.lo)),
             _mm_cmpeq_epi32(a.components.hi, _mm_min_epu32(a.components.hi, b.components.hi))}};
#endif
  }

  v_ui32_8 operator>=(const v_ui32_8 &a, const v_ui32_8 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_cmpeq_epi32(a.components, _mm256_max_epu32(a.components, b.components));
#elif VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    const auto alo = _mm256_castsi256_si128(a.components);
    const auto ahi = _mm256_extractf128_si256(a.components, 1);
    const auto blo = _mm256_castsi256_si128(b.components);
    const auto bhi = _mm256_extractf128_si256(b.components, 1);
    const auto lo = _mm_cmpeq_epi32(alo, _mm_max_epu32(alo, blo));
    const auto hi = _mm_cmpeq_epi32(ahi, _mm_max_epu32(ahi, bhi));
    return _mm256_insertf128_si256(_mm256_castsi128_si256(lo), hi, 1);
#else
    return {{_mm_cmpeq_epi32(a.components.lo, _mm_max_epu32(a.components.lo, b.components.lo)),
             _mm_cmpeq_epi32(a.components.hi, _mm_max_epu32(a.components.hi, b.components.hi))}};
#endif
  }

  v_ui32_8 operator<(const v_ui32_8 &a, const v_ui32_8 &b) noexcept {
    return ~(a >= b);
  }

  v_ui32_8 operator>(const v_ui32_8 &a, const v_ui32_8 &b) noexcept {
    return ~(a <= b);
  }

  v_ui32_8 operator==(const v_ui32_8 &a, const v_ui32_8 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_cmpeq_epi32(a.components, b.components);
#elif VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    const auto lo = _mm_cmpeq_epi32(_mm256_castsi256_si128(a.components), _mm256_castsi256_si128(b.components));
    const auto hi = _mm_cmpeq_epi32(_mm256_extractf128_si256(a.components, 1), _mm256_extractf128_si256(b.components, 1));
    return _mm256_insertf128_si256(_mm256_castsi128_si256(lo), hi, 1);
#else
    return {{_mm_cmpeq_epi32(a.components.lo, b.components.lo), _mm_cmpeq_epi32(a.components.hi, b.components.hi)}};
#endif
  }

  v_ui32_8 operator!=(const v_ui32_8 &a, const v_ui32_8 &b) noexcept {
    return ~(a == b);
  }

  v_ui32_8 &operator+=(v_ui32_8 &a, const v_ui32_8 &b) noexcept { return a = a + b; }
  v_ui32_8 &operator-=(v_ui32_8 &a, const v_ui32_8 &b) noexcept { return a = a - b; }
  v_ui32_8 &operator*=(v_ui32_8 &a, const v_ui32_8 &b) noexcept { return a = a * b; }
  v_ui32_8 &operator&=(v_ui32_8 &a, const v_ui32_8 &b) noexcept { return a = a & b; }
  v_ui32_8 &operator|=(v_ui32_8 &a, const v_ui32_8 &b) noexcept { return a = a | b; }
  v_ui32_8 &operator^=(v_ui32_8 &a, const v_ui32_8 &b) noexcept { return a = a ^ b; }
  v_ui32_8 &operator<<=(v_ui32_8 &a, const Int_32 count) noexcept { return a = a << count; }
  v_ui32_8 &operator<<=(v_ui32_8 &a, const UInt_32 count) noexcept { return a = a << count; }
  v_ui32_8 &operator<<=(v_ui32_8 &a, const v_i32_8 &b) noexcept { return a = a << b; }
  v_ui32_8 &operator<<=(v_ui32_8 &a, const v_ui32_8 &b) noexcept { return a = a << b; }

  v_ui32_8 &operator>>=(v_ui32_8 &a, const Int_32 count) noexcept { return a = a >> count; }
  v_ui32_8 &operator>>=(v_ui32_8 &a, const UInt_32 count) noexcept { return a = a >> count; }
  v_ui32_8 &operator>>=(v_ui32_8 &a, const v_i32_8 &b) noexcept { return a = a >> b; }
  v_ui32_8 &operator>>=(v_ui32_8 &a, const v_ui32_8 &b) noexcept { return a = a >> b; }

  std::ostream &operator<<(std::ostream &stream, const v_ui32_8 &v) {
    return stream << "{" << x1(v) << ", " << x2(v) << ", " << x3(v) << ", " << x4(v)
                  << ", " << x5(v) << ", " << x6(v) << ", " << x7(v) << ", " << x8(v) << "}";
  }
}
