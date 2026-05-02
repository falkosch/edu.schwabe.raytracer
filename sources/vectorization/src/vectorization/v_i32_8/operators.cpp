#include "vectorization/v_i32_8/operators.h"

#include "vectorization/v_i32_8/accessors.h"
#include "vectorization/v_i32_8/constants.h"

namespace vectorization {
  v_i32_8 operator-(const v_i32_8 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_sub_epi32(Zero<v_i32_8::PackedType>(), v.components);
#elif VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    const auto z = Zero<PackedInts_128>();
    const auto lo = _mm_sub_epi32(z, _mm256_castsi256_si128(v.components));
    const auto hi = _mm_sub_epi32(z, _mm256_extractf128_si256(v.components, 1));
    return _mm256_insertf128_si256(_mm256_castsi128_si256(lo), hi, 1);
#else
    const auto z = Zero<v_i32_8::HalfPackedType>();
    return {{_mm_sub_epi32(z, v.components.lo), _mm_sub_epi32(z, v.components.hi)}};
#endif
  }

  v_i32_8 operator!(const v_i32_8 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_cmpeq_epi32(v.components, Zero<v_i32_8::PackedType>());
#elif VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    const auto z = Zero<PackedInts_128>();
    const auto lo = _mm_cmpeq_epi32(_mm256_castsi256_si128(v.components), z);
    const auto hi = _mm_cmpeq_epi32(_mm256_extractf128_si256(v.components, 1), z);
    return _mm256_insertf128_si256(_mm256_castsi128_si256(lo), hi, 1);
#else
    const auto z = Zero<v_i32_8::HalfPackedType>();
    return {{_mm_cmpeq_epi32(v.components.lo, z), _mm_cmpeq_epi32(v.components.hi, z)}};
#endif
  }

  v_i32_8 operator~(const v_i32_8 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    const auto allOnes = _mm256_cmpeq_epi32(v.components, v.components);
    return _mm256_xor_si256(v.components, allOnes);
#elif VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    const auto lo = _mm256_castsi256_si128(v.components);
    const auto hi = _mm256_extractf128_si256(v.components, 1);
    const auto allOnes = _mm_cmpeq_epi32(lo, lo);
    const auto rlo = _mm_xor_si128(lo, allOnes);
    const auto rhi = _mm_xor_si128(hi, allOnes);
    return _mm256_insertf128_si256(_mm256_castsi128_si256(rlo), rhi, 1);
#else
    const auto allOnes = _mm_cmpeq_epi32(v.components.lo, v.components.lo);
    return {{_mm_xor_si128(v.components.lo, allOnes), _mm_xor_si128(v.components.hi, allOnes)}};
#endif
  }

  v_i32_8 operator+(const v_i32_8 &a, const v_i32_8 &b) noexcept {
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

  v_i32_8 operator-(const v_i32_8 &a, const v_i32_8 &b) noexcept {
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

  v_i32_8 operator*(const v_i32_8 &a, const v_i32_8 &b) noexcept {
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

  v_i32_8 operator&(const v_i32_8 &a, const v_i32_8 &b) noexcept {
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

  v_i32_8 operator|(const v_i32_8 &a, const v_i32_8 &b) noexcept {
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

  v_i32_8 operator^(const v_i32_8 &a, const v_i32_8 &b) noexcept {
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

  v_i32_8 operator<<(const v_i32_8 &a, const v_i32_8 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_sllv_epi32(a.components, b.components);
#else
    v_i32_8 result;
    for (ASizeT i = 0; i < v_i32_8::SIZE; ++i)
      result[i] = a[i] << b[i];
    return result;
#endif
  }

  v_i32_8 operator>>(const v_i32_8 &a, const v_i32_8 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_srav_epi32(a.components, b.components);
#else
    v_i32_8 result;
    for (ASizeT i = 0; i < v_i32_8::SIZE; ++i)
      result[i] = a[i] >> b[i];
    return result;
#endif
  }

  v_i32_8 operator<(const v_i32_8 &a, const v_i32_8 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_cmpgt_epi32(b.components, a.components);
#elif VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    const auto lo = _mm_cmpgt_epi32(_mm256_castsi256_si128(b.components), _mm256_castsi256_si128(a.components));
    const auto hi = _mm_cmpgt_epi32(_mm256_extractf128_si256(b.components, 1), _mm256_extractf128_si256(a.components, 1));
    return _mm256_insertf128_si256(_mm256_castsi128_si256(lo), hi, 1);
#else
    return {{_mm_cmpgt_epi32(b.components.lo, a.components.lo), _mm_cmpgt_epi32(b.components.hi, a.components.hi)}};
#endif
  }

  v_i32_8 operator>(const v_i32_8 &a, const v_i32_8 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_cmpgt_epi32(a.components, b.components);
#elif VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    const auto lo = _mm_cmpgt_epi32(_mm256_castsi256_si128(a.components), _mm256_castsi256_si128(b.components));
    const auto hi = _mm_cmpgt_epi32(_mm256_extractf128_si256(a.components, 1), _mm256_extractf128_si256(b.components, 1));
    return _mm256_insertf128_si256(_mm256_castsi128_si256(lo), hi, 1);
#else
    return {{_mm_cmpgt_epi32(a.components.lo, b.components.lo), _mm_cmpgt_epi32(a.components.hi, b.components.hi)}};
#endif
  }

  v_i32_8 operator==(const v_i32_8 &a, const v_i32_8 &b) noexcept {
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

  v_i32_8 operator!=(const v_i32_8 &a, const v_i32_8 &b) noexcept {
    return ~(a == b);
  }

  v_i32_8 &operator+=(v_i32_8 &a, const v_i32_8 &b) noexcept { return a = a + b; }
  v_i32_8 &operator-=(v_i32_8 &a, const v_i32_8 &b) noexcept { return a = a - b; }
  v_i32_8 &operator*=(v_i32_8 &a, const v_i32_8 &b) noexcept { return a = a * b; }
  v_i32_8 &operator&=(v_i32_8 &a, const v_i32_8 &b) noexcept { return a = a & b; }
  v_i32_8 &operator|=(v_i32_8 &a, const v_i32_8 &b) noexcept { return a = a | b; }
  v_i32_8 &operator^=(v_i32_8 &a, const v_i32_8 &b) noexcept { return a = a ^ b; }
  v_i32_8 &operator<<=(v_i32_8 &a, const v_i32_8 &b) noexcept { return a = a << b; }
  v_i32_8 &operator>>=(v_i32_8 &a, const v_i32_8 &b) noexcept { return a = a >> b; }

  std::ostream &operator<<(std::ostream &stream, const v_i32_8 &v) {
    return stream << "{" << x1(v) << ", " << x2(v) << ", " << x3(v) << ", " << x4(v)
                  << ", " << x5(v) << ", " << x6(v) << ", " << x7(v) << ", " << x8(v) << "}";
  }
}
