#include "vectorization/v_f32_8/operators.h"

#include "vectorization/v_f32_8/accessors.h"
#include "vectorization/v_f32_8/constants.h"

#include <bit>

#include "vectorization/functions/add.h"
#include "vectorization/functions/divide.h"
#include "vectorization/functions/modulo.h"
#include "vectorization/functions/multiply.h"
#include "vectorization/functions/subtract.h"

namespace vectorization {
  v_f32_8 operator-(const v_f32_8 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return _mm256_xor_ps(NegativeZero<v_f32_8::PackedType>(), v.components);
#else
    const auto nz = NegativeZero<v_f32_8::HalfPackedType>();
    return {{_mm_xor_ps(nz, v.components.lo), _mm_xor_ps(nz, v.components.hi)}};
#endif
  }

  v_f32_8 operator~(const v_f32_8 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return _mm256_xor_ps(MaskAll<v_f32_8::PackedType>(), v.components);
#else
    const auto m = MaskAll<v_f32_8::HalfPackedType>();
    return {{_mm_xor_ps(m, v.components.lo), _mm_xor_ps(m, v.components.hi)}};
#endif
  }

  v_f32_8::VectorBoolType operator!(const v_f32_8 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return _mm256_castps_si256(_mm256_cmp_ps(v.components, Zero<v_f32_8::PackedType>(), _CMP_EQ_OQ));
#else
    const auto z = Zero<v_f32_8::HalfPackedType>();
    return {{_mm_castps_si128(_mm_cmpeq_ps(v.components.lo, z)),
             _mm_castps_si128(_mm_cmpeq_ps(v.components.hi, z))}};
#endif
  }

  v_f32_8 operator+(const v_f32_8 &a, const v_f32_8 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return add(a.components, b.components);
#else
    return {{_mm_add_ps(a.components.lo, b.components.lo), _mm_add_ps(a.components.hi, b.components.hi)}};
#endif
  }

  v_f32_8 operator+(const v_f32_8 &a, const v_f32_8::ValueType &b) noexcept {
    return a + v_f32_8(b);
  }

  v_f32_8 operator+(const v_f32_8::ValueType &a, const v_f32_8 &b) noexcept {
    return v_f32_8(a) + b;
  }

  v_f32_8 operator-(const v_f32_8 &a, const v_f32_8 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return subtract(a.components, b.components);
#else
    return {{_mm_sub_ps(a.components.lo, b.components.lo), _mm_sub_ps(a.components.hi, b.components.hi)}};
#endif
  }

  v_f32_8 operator-(const v_f32_8 &a, const v_f32_8::ValueType &b) noexcept {
    return a - v_f32_8(b);
  }

  v_f32_8 operator-(const v_f32_8::ValueType &a, const v_f32_8 &b) noexcept {
    return v_f32_8(a) - b;
  }

  v_f32_8 operator*(const v_f32_8 &a, const v_f32_8 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return multiply(a.components, b.components);
#else
    return {{_mm_mul_ps(a.components.lo, b.components.lo), _mm_mul_ps(a.components.hi, b.components.hi)}};
#endif
  }

  v_f32_8 operator*(const v_f32_8 &a, const v_f32_8::ValueType &b) noexcept {
    return a * v_f32_8(b);
  }

  v_f32_8 operator*(const v_f32_8::ValueType &a, const v_f32_8 &b) noexcept {
    return v_f32_8(a) * b;
  }

  v_f32_8 operator/(const v_f32_8 &a, const v_f32_8 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return divide(a.components, b.components);
#else
    return {{_mm_div_ps(a.components.lo, b.components.lo), _mm_div_ps(a.components.hi, b.components.hi)}};
#endif
  }

  v_f32_8 operator/(const v_f32_8 &a, const v_f32_8::ValueType &b) noexcept {
    return a / v_f32_8(b);
  }

  v_f32_8 operator/(const v_f32_8::ValueType &a, const v_f32_8 &b) noexcept {
    return v_f32_8(a) / b;
  }

  v_f32_8 operator%(const v_f32_8 &a, const v_f32_8 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return modulo(a.components, b.components);
#else
    return {{modulo(a.components.lo, b.components.lo), modulo(a.components.hi, b.components.hi)}};
#endif
  }

  v_f32_8 operator&(const v_f32_8 &a, const v_f32_8 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return _mm256_and_ps(a.components, b.components);
#else
    return {{_mm_and_ps(a.components.lo, b.components.lo), _mm_and_ps(a.components.hi, b.components.hi)}};
#endif
  }

  v_f32_8 operator|(const v_f32_8 &a, const v_f32_8 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return _mm256_or_ps(a.components, b.components);
#else
    return {{_mm_or_ps(a.components.lo, b.components.lo), _mm_or_ps(a.components.hi, b.components.hi)}};
#endif
  }

  v_f32_8 operator^(const v_f32_8 &a, const v_f32_8 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return _mm256_xor_ps(a.components, b.components);
#else
    return {{_mm_xor_ps(a.components.lo, b.components.lo), _mm_xor_ps(a.components.hi, b.components.hi)}};
#endif
  }

  v_f32_8 operator<<(const v_f32_8 &a, const v_f32_8::VectorBoolType &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_castsi256_ps(_mm256_sllv_epi32(_mm256_castps_si256(a.components), b.components));
#else
    v_f32_8 result;
    for (ASizeT i = 0; i < v_f32_8::SIZE; ++i) {
      const auto bits = std::bit_cast<Int_32>(a[i]);
      result[i] = std::bit_cast<Float_32>(bits << b[i]);
    }
    return result;
#endif
  }

  v_f32_8 operator>>(const v_f32_8 &a, const v_f32_8::VectorBoolType &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_castsi256_ps(_mm256_srlv_epi32(_mm256_castps_si256(a.components), b.components));
#else
    v_f32_8 result;
    for (ASizeT i = 0; i < v_f32_8::SIZE; ++i) {
      const auto bits = std::bit_cast<Int_32>(a[i]);
      result[i] = std::bit_cast<Float_32>(static_cast<Int_32>(static_cast<UInt_32>(bits) >> b[i]));
    }
    return result;
#endif
  }

  v_f32_8 &operator+=(v_f32_8 &a, const v_f32_8 &b) noexcept { return a = a + b; }
  v_f32_8 &operator-=(v_f32_8 &a, const v_f32_8 &b) noexcept { return a = a - b; }
  v_f32_8 &operator*=(v_f32_8 &a, const v_f32_8 &b) noexcept { return a = a * b; }
  v_f32_8 &operator/=(v_f32_8 &a, const v_f32_8 &b) noexcept { return a = a / b; }
  v_f32_8 &operator%=(v_f32_8 &a, const v_f32_8 &b) noexcept { return a = a % b; }
  v_f32_8 &operator&=(v_f32_8 &a, const v_f32_8 &b) noexcept { return a = a & b; }
  v_f32_8 &operator|=(v_f32_8 &a, const v_f32_8 &b) noexcept { return a = a | b; }
  v_f32_8 &operator^=(v_f32_8 &a, const v_f32_8 &b) noexcept { return a = a ^ b; }
  v_f32_8 &operator<<=(v_f32_8 &a, const v_f32_8::VectorBoolType &b) noexcept { return a = a << b; }
  v_f32_8 &operator>>=(v_f32_8 &a, const v_f32_8::VectorBoolType &b) noexcept { return a = a >> b; }

  v_f32_8::VectorBoolType operator<(const v_f32_8 &a, const v_f32_8 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return _mm256_castps_si256(_mm256_cmp_ps(a.components, b.components, _CMP_LT_OS));
#else
    return {{_mm_castps_si128(_mm_cmplt_ps(a.components.lo, b.components.lo)),
             _mm_castps_si128(_mm_cmplt_ps(a.components.hi, b.components.hi))}};
#endif
  }

  v_f32_8::VectorBoolType operator>(const v_f32_8 &a, const v_f32_8 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return _mm256_castps_si256(_mm256_cmp_ps(a.components, b.components, _CMP_GT_OS));
#else
    return {{_mm_castps_si128(_mm_cmpgt_ps(a.components.lo, b.components.lo)),
             _mm_castps_si128(_mm_cmpgt_ps(a.components.hi, b.components.hi))}};
#endif
  }

  v_f32_8::VectorBoolType operator<=(const v_f32_8 &a, const v_f32_8 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return _mm256_castps_si256(_mm256_cmp_ps(a.components, b.components, _CMP_LE_OS));
#else
    return {{_mm_castps_si128(_mm_cmple_ps(a.components.lo, b.components.lo)),
             _mm_castps_si128(_mm_cmple_ps(a.components.hi, b.components.hi))}};
#endif
  }

  v_f32_8::VectorBoolType operator>=(const v_f32_8 &a, const v_f32_8 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return _mm256_castps_si256(_mm256_cmp_ps(a.components, b.components, _CMP_GE_OS));
#else
    return {{_mm_castps_si128(_mm_cmpge_ps(a.components.lo, b.components.lo)),
             _mm_castps_si128(_mm_cmpge_ps(a.components.hi, b.components.hi))}};
#endif
  }

  v_f32_8::VectorBoolType operator==(const v_f32_8 &a, const v_f32_8 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return _mm256_castps_si256(_mm256_cmp_ps(a.components, b.components, _CMP_EQ_OQ));
#else
    return {{_mm_castps_si128(_mm_cmpeq_ps(a.components.lo, b.components.lo)),
             _mm_castps_si128(_mm_cmpeq_ps(a.components.hi, b.components.hi))}};
#endif
  }

  v_f32_8::VectorBoolType operator!=(const v_f32_8 &a, const v_f32_8 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return _mm256_castps_si256(_mm256_cmp_ps(a.components, b.components, _CMP_NEQ_UQ));
#else
    return {{_mm_castps_si128(_mm_cmpneq_ps(a.components.lo, b.components.lo)),
             _mm_castps_si128(_mm_cmpneq_ps(a.components.hi, b.components.hi))}};
#endif
  }

  std::ostream &operator<<(std::ostream &stream, const v_f32_8 &v) {
    return stream << "{" << x1(v) << ", " << x2(v) << ", " << x3(v) << ", " << x4(v) << ", " << x5(v) << ", " << x6(v)
                  << ", " << x7(v) << ", " << x8(v) << "}";
  }
}
