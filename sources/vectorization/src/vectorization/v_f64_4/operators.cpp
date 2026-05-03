#include "vectorization/v_f64_4/operators.h"

#include "vectorization/v_f64_4/accessors.h"
#include "vectorization/v_f64_4/constants.h"

#if VECTORIZATION_INTRINSICS_LEVEL < VECTORIZATION_AVX2
#include <bit>
#endif

#include "vectorization/functions/add.h"
#include "vectorization/functions/divide.h"
#include "vectorization/functions/modulo.h"
#include "vectorization/functions/multiply.h"
#include "vectorization/functions/subtract.h"

namespace vectorization {
  v_f64_4 operator-(const v_f64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return _mm256_xor_pd(NegativeZero<v_f64_4::PackedType>(), v.components);
#else
    const auto nz = NegativeZero<v_f64_4::HalfPackedType>();
    return {{_mm_xor_pd(nz, v.components.lo), _mm_xor_pd(nz, v.components.hi)}};
#endif
  }

  v_f64_4 operator~(const v_f64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return _mm256_xor_pd(MaskAll<v_f64_4::PackedType>(), v.components);
#else
    const auto m = MaskAll<v_f64_4::HalfPackedType>();
    return {{_mm_xor_pd(m, v.components.lo), _mm_xor_pd(m, v.components.hi)}};
#endif
  }

  v_f64_4::VectorBoolType operator!(const v_f64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return _mm256_castpd_si256(_mm256_cmp_pd(v.components, Zero<v_f64_4::PackedType>(), _CMP_EQ_OQ));
#else
    const auto z = Zero<v_f64_4::HalfPackedType>();
    return {{_mm_castpd_si128(_mm_cmpeq_pd(v.components.lo, z)),
             _mm_castpd_si128(_mm_cmpeq_pd(v.components.hi, z))}};
#endif
  }

  v_f64_4 operator+(const v_f64_4 &a, const v_f64_4 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return add(a.components, b.components);
#else
    return {{_mm_add_pd(a.components.lo, b.components.lo), _mm_add_pd(a.components.hi, b.components.hi)}};
#endif
  }

  v_f64_4 operator+(const v_f64_4 &a, const v_f64_4::ValueType &b) noexcept {
    return a + v_f64_4(b);
  }

  v_f64_4 operator+(const v_f64_4::ValueType &a, const v_f64_4 &b) noexcept {
    return v_f64_4(a) + b;
  }

  v_f64_4 operator-(const v_f64_4 &a, const v_f64_4 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return subtract(a.components, b.components);
#else
    return {{_mm_sub_pd(a.components.lo, b.components.lo), _mm_sub_pd(a.components.hi, b.components.hi)}};
#endif
  }

  v_f64_4 operator-(const v_f64_4 &a, const v_f64_4::ValueType &b) noexcept {
    return a - v_f64_4(b);
  }

  v_f64_4 operator-(const v_f64_4::ValueType &a, const v_f64_4 &b) noexcept {
    return v_f64_4(a) - b;
  }

  v_f64_4 operator*(const v_f64_4 &a, const v_f64_4 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return multiply(a.components, b.components);
#else
    return {{_mm_mul_pd(a.components.lo, b.components.lo), _mm_mul_pd(a.components.hi, b.components.hi)}};
#endif
  }

  v_f64_4 operator*(const v_f64_4 &a, const v_f64_4::ValueType &b) noexcept {
    return a * v_f64_4(b);
  }

  v_f64_4 operator*(const v_f64_4::ValueType &a, const v_f64_4 &b) noexcept {
    return v_f64_4(a) * b;
  }

  v_f64_4 operator/(const v_f64_4 &a, const v_f64_4 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return divide(a.components, b.components);
#else
    return {{_mm_div_pd(a.components.lo, b.components.lo), _mm_div_pd(a.components.hi, b.components.hi)}};
#endif
  }

  v_f64_4 operator/(const v_f64_4 &a, const v_f64_4::ValueType &b) noexcept {
    return a / v_f64_4(b);
  }

  v_f64_4 operator/(const v_f64_4::ValueType &a, const v_f64_4 &b) noexcept {
    return v_f64_4(a) / b;
  }

  v_f64_4 operator%(const v_f64_4 &a, const v_f64_4 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return modulo(a.components, b.components);
#else
    return {{modulo(a.components.lo, b.components.lo), modulo(a.components.hi, b.components.hi)}};
#endif
  }

  v_f64_4 operator&(const v_f64_4 &a, const v_f64_4 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return _mm256_and_pd(a.components, b.components);
#else
    return {{_mm_and_pd(a.components.lo, b.components.lo), _mm_and_pd(a.components.hi, b.components.hi)}};
#endif
  }

  v_f64_4 operator|(const v_f64_4 &a, const v_f64_4 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return _mm256_or_pd(a.components, b.components);
#else
    return {{_mm_or_pd(a.components.lo, b.components.lo), _mm_or_pd(a.components.hi, b.components.hi)}};
#endif
  }

  v_f64_4 operator^(const v_f64_4 &a, const v_f64_4 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return _mm256_xor_pd(a.components, b.components);
#else
    return {{_mm_xor_pd(a.components.lo, b.components.lo), _mm_xor_pd(a.components.hi, b.components.hi)}};
#endif
  }

  v_f64_4 operator<<(const v_f64_4 &a, const v_f64_4::VectorBoolType &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_castsi256_pd(_mm256_sllv_epi64(_mm256_castpd_si256(a.components), b.components));
#else
    v_f64_4 result;
    for (ASizeT i = 0; i < v_f64_4::SIZE; ++i) {
      const auto bits = std::bit_cast<Int_64>(a[i]);
      result[i] = std::bit_cast<Float_64>(bits << b[i]);
    }
    return result;
#endif
  }

  v_f64_4 operator>>(const v_f64_4 &a, const v_f64_4::VectorBoolType &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_castsi256_pd(_mm256_srlv_epi64(_mm256_castpd_si256(a.components), b.components));
#else
    v_f64_4 result;
    for (ASizeT i = 0; i < v_f64_4::SIZE; ++i) {
      const auto bits = std::bit_cast<Int_64>(a[i]);
      result[i] = std::bit_cast<Float_64>(static_cast<Int_64>(static_cast<UInt_64>(bits) >> b[i]));
    }
    return result;
#endif
  }

  v_f64_4 &operator+=(v_f64_4 &a, const v_f64_4 &b) noexcept { return a = a + b; }
  v_f64_4 &operator-=(v_f64_4 &a, const v_f64_4 &b) noexcept { return a = a - b; }
  v_f64_4 &operator*=(v_f64_4 &a, const v_f64_4 &b) noexcept { return a = a * b; }
  v_f64_4 &operator/=(v_f64_4 &a, const v_f64_4 &b) noexcept { return a = a / b; }
  v_f64_4 &operator%=(v_f64_4 &a, const v_f64_4 &b) noexcept { return a = a % b; }
  v_f64_4 &operator&=(v_f64_4 &a, const v_f64_4 &b) noexcept { return a = a & b; }
  v_f64_4 &operator|=(v_f64_4 &a, const v_f64_4 &b) noexcept { return a = a | b; }
  v_f64_4 &operator^=(v_f64_4 &a, const v_f64_4 &b) noexcept { return a = a ^ b; }
  v_f64_4 &operator<<=(v_f64_4 &a, const v_f64_4::VectorBoolType &b) noexcept { return a = a << b; }
  v_f64_4 &operator>>=(v_f64_4 &a, const v_f64_4::VectorBoolType &b) noexcept { return a = a >> b; }

  v_f64_4::VectorBoolType operator<(const v_f64_4 &a, const v_f64_4 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return _mm256_castpd_si256(_mm256_cmp_pd(a.components, b.components, _CMP_LT_OS));
#else
    return {{_mm_castpd_si128(_mm_cmplt_pd(a.components.lo, b.components.lo)),
             _mm_castpd_si128(_mm_cmplt_pd(a.components.hi, b.components.hi))}};
#endif
  }

  v_f64_4::VectorBoolType operator>(const v_f64_4 &a, const v_f64_4 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return _mm256_castpd_si256(_mm256_cmp_pd(a.components, b.components, _CMP_GT_OS));
#else
    return {{_mm_castpd_si128(_mm_cmpgt_pd(a.components.lo, b.components.lo)),
             _mm_castpd_si128(_mm_cmpgt_pd(a.components.hi, b.components.hi))}};
#endif
  }

  v_f64_4::VectorBoolType operator<=(const v_f64_4 &a, const v_f64_4 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return _mm256_castpd_si256(_mm256_cmp_pd(a.components, b.components, _CMP_LE_OS));
#else
    return {{_mm_castpd_si128(_mm_cmple_pd(a.components.lo, b.components.lo)),
             _mm_castpd_si128(_mm_cmple_pd(a.components.hi, b.components.hi))}};
#endif
  }

  v_f64_4::VectorBoolType operator>=(const v_f64_4 &a, const v_f64_4 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return _mm256_castpd_si256(_mm256_cmp_pd(a.components, b.components, _CMP_GE_OS));
#else
    return {{_mm_castpd_si128(_mm_cmpge_pd(a.components.lo, b.components.lo)),
             _mm_castpd_si128(_mm_cmpge_pd(a.components.hi, b.components.hi))}};
#endif
  }

  v_f64_4::VectorBoolType operator==(const v_f64_4 &a, const v_f64_4 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return _mm256_castpd_si256(_mm256_cmp_pd(a.components, b.components, _CMP_EQ_OQ));
#else
    return {{_mm_castpd_si128(_mm_cmpeq_pd(a.components.lo, b.components.lo)),
             _mm_castpd_si128(_mm_cmpeq_pd(a.components.hi, b.components.hi))}};
#endif
  }

  v_f64_4::VectorBoolType operator!=(const v_f64_4 &a, const v_f64_4 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return _mm256_castpd_si256(_mm256_cmp_pd(a.components, b.components, _CMP_NEQ_UQ));
#else
    return {{_mm_castpd_si128(_mm_cmpneq_pd(a.components.lo, b.components.lo)),
             _mm_castpd_si128(_mm_cmpneq_pd(a.components.hi, b.components.hi))}};
#endif
  }

  std::ostream &operator<<(std::ostream &stream, const v_f64_4 &v) {
    return stream << "{" << x(v) << ", " << y(v) << ", " << z(v) << ", " << w(v) << "}";
  }
}
