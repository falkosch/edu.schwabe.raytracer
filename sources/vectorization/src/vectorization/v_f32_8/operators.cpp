#include "vectorization/v_f32_8/operators.h"

#include "vectorization/v_f32_8/accessors.h"

#include "vectorization/constants/values.h"
#include "vectorization/functions/add.h"
#include "vectorization/functions/bitwise.h"
#include "vectorization/functions/compare.h"
#include "vectorization/functions/divide.h"
#include "vectorization/functions/modulo.h"
#include "vectorization/functions/multiply.h"
#include "vectorization/functions/negate.h"
#include "vectorization/functions/shift.h"
#include "vectorization/functions/subtract.h"
#include "vectorization/v_ui32_8/type.h"

namespace vectorization {
  v_f32_8 operator-(const v_f32_8 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return negate(v.components);
#else
    return {{negate(v.components.lo), negate(v.components.hi)}};
#endif
  }

  v_f32_8 operator~(const v_f32_8 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return bitwiseNot(v.components);
#else
    return {{bitwiseNot(v.components.lo), bitwiseNot(v.components.hi)}};
#endif
  }

  v_f32_8::VectorBoolType operator!(const v_f32_8 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return _mm256_castps_si256(compareEqual(v.components, Zero<v_f32_8::PackedType>()));
#else
    const auto z = Zero<v_f32_8::HalfPackedType>();
    return {{_mm_castps_si128(compareEqual(v.components.lo, z)),
             _mm_castps_si128(compareEqual(v.components.hi, z))}};
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
    return bitwiseAnd(a.components, b.components);
#else
    return {{bitwiseAnd(a.components.lo, b.components.lo), bitwiseAnd(a.components.hi, b.components.hi)}};
#endif
  }

  v_f32_8 operator|(const v_f32_8 &a, const v_f32_8 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return bitwiseOr(a.components, b.components);
#else
    return {{bitwiseOr(a.components.lo, b.components.lo), bitwiseOr(a.components.hi, b.components.hi)}};
#endif
  }

  v_f32_8 operator^(const v_f32_8 &a, const v_f32_8 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return bitwiseXor(a.components, b.components);
#else
    return {{bitwiseXor(a.components.lo, b.components.lo), bitwiseXor(a.components.hi, b.components.hi)}};
#endif
  }

  v_f32_8 operator<<(const v_f32_8 &a, const Int_32 count) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return _mm256_castsi256_ps(shiftLeft32(_mm256_castps_si256(a.components), count));
#else
    return {{_mm_castsi128_ps(shiftLeft32(_mm_castps_si128(a.components.lo), count)),
             _mm_castsi128_ps(shiftLeft32(_mm_castps_si128(a.components.hi), count))}};
#endif
  }

  v_f32_8 operator<<(const v_f32_8 &a, const UInt_32 count) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return _mm256_castsi256_ps(shiftLeft32(_mm256_castps_si256(a.components), count));
#else
    return {{_mm_castsi128_ps(shiftLeft32(_mm_castps_si128(a.components.lo), count)),
             _mm_castsi128_ps(shiftLeft32(_mm_castps_si128(a.components.hi), count))}};
#endif
  }

  v_f32_8 operator<<(const v_f32_8 &a, const v_i32_8 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return _mm256_castsi256_ps(shiftLeft32(_mm256_castps_si256(a.components), b.components));
#else
    return {{_mm_castsi128_ps(shiftLeft32(_mm_castps_si128(a.components.lo), b.components.lo)),
             _mm_castsi128_ps(shiftLeft32(_mm_castps_si128(a.components.hi), b.components.hi))}};
#endif
  }

  v_f32_8 operator<<(const v_f32_8 &a, const v_ui32_8 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return _mm256_castsi256_ps(shiftLeft32(_mm256_castps_si256(a.components), b.components));
#else
    return {{_mm_castsi128_ps(shiftLeft32(_mm_castps_si128(a.components.lo), b.components.lo)),
             _mm_castsi128_ps(shiftLeft32(_mm_castps_si128(a.components.hi), b.components.hi))}};
#endif
  }

  v_f32_8 operator>>(const v_f32_8 &a, const Int_32 count) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return _mm256_castsi256_ps(shiftRightLogical32(_mm256_castps_si256(a.components), count));
#else
    return {{_mm_castsi128_ps(shiftRightLogical32(_mm_castps_si128(a.components.lo), count)),
             _mm_castsi128_ps(shiftRightLogical32(_mm_castps_si128(a.components.hi), count))}};
#endif
  }

  v_f32_8 operator>>(const v_f32_8 &a, const UInt_32 count) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return _mm256_castsi256_ps(shiftRightLogical32(_mm256_castps_si256(a.components), count));
#else
    return {{_mm_castsi128_ps(shiftRightLogical32(_mm_castps_si128(a.components.lo), count)),
             _mm_castsi128_ps(shiftRightLogical32(_mm_castps_si128(a.components.hi), count))}};
#endif
  }

  v_f32_8 operator>>(const v_f32_8 &a, const v_i32_8 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return _mm256_castsi256_ps(shiftRightLogical32(_mm256_castps_si256(a.components), b.components));
#else
    return {{_mm_castsi128_ps(shiftRightLogical32(_mm_castps_si128(a.components.lo), b.components.lo)),
             _mm_castsi128_ps(shiftRightLogical32(_mm_castps_si128(a.components.hi), b.components.hi))}};
#endif
  }

  v_f32_8 operator>>(const v_f32_8 &a, const v_ui32_8 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return _mm256_castsi256_ps(shiftRightLogical32(_mm256_castps_si256(a.components), b.components));
#else
    return {{_mm_castsi128_ps(shiftRightLogical32(_mm_castps_si128(a.components.lo), b.components.lo)),
             _mm_castsi128_ps(shiftRightLogical32(_mm_castps_si128(a.components.hi), b.components.hi))}};
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
  v_f32_8 &operator<<=(v_f32_8 &a, const Int_32 count) noexcept { return a = a << count; }
  v_f32_8 &operator<<=(v_f32_8 &a, const UInt_32 count) noexcept { return a = a << count; }
  v_f32_8 &operator<<=(v_f32_8 &a, const v_i32_8 &b) noexcept { return a = a << b; }
  v_f32_8 &operator<<=(v_f32_8 &a, const v_ui32_8 &b) noexcept { return a = a << b; }

  v_f32_8 &operator>>=(v_f32_8 &a, const Int_32 count) noexcept { return a = a >> count; }
  v_f32_8 &operator>>=(v_f32_8 &a, const UInt_32 count) noexcept { return a = a >> count; }
  v_f32_8 &operator>>=(v_f32_8 &a, const v_i32_8 &b) noexcept { return a = a >> b; }
  v_f32_8 &operator>>=(v_f32_8 &a, const v_ui32_8 &b) noexcept { return a = a >> b; }

  v_f32_8::VectorBoolType operator<(const v_f32_8 &a, const v_f32_8 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return _mm256_castps_si256(compareLess(a.components, b.components));
#else
    return {{_mm_castps_si128(compareLess(a.components.lo, b.components.lo)),
             _mm_castps_si128(compareLess(a.components.hi, b.components.hi))}};
#endif
  }

  v_f32_8::VectorBoolType operator>(const v_f32_8 &a, const v_f32_8 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return _mm256_castps_si256(compareGreater(a.components, b.components));
#else
    return {{_mm_castps_si128(compareGreater(a.components.lo, b.components.lo)),
             _mm_castps_si128(compareGreater(a.components.hi, b.components.hi))}};
#endif
  }

  v_f32_8::VectorBoolType operator<=(const v_f32_8 &a, const v_f32_8 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return _mm256_castps_si256(compareLessEqual(a.components, b.components));
#else
    return {{_mm_castps_si128(compareLessEqual(a.components.lo, b.components.lo)),
             _mm_castps_si128(compareLessEqual(a.components.hi, b.components.hi))}};
#endif
  }

  v_f32_8::VectorBoolType operator>=(const v_f32_8 &a, const v_f32_8 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return _mm256_castps_si256(compareGreaterEqual(a.components, b.components));
#else
    return {{_mm_castps_si128(compareGreaterEqual(a.components.lo, b.components.lo)),
             _mm_castps_si128(compareGreaterEqual(a.components.hi, b.components.hi))}};
#endif
  }

  v_f32_8::VectorBoolType operator==(const v_f32_8 &a, const v_f32_8 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return _mm256_castps_si256(compareEqual(a.components, b.components));
#else
    return {{_mm_castps_si128(compareEqual(a.components.lo, b.components.lo)),
             _mm_castps_si128(compareEqual(a.components.hi, b.components.hi))}};
#endif
  }

  v_f32_8::VectorBoolType operator!=(const v_f32_8 &a, const v_f32_8 &b) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return _mm256_castps_si256(compareNotEqual(a.components, b.components));
#else
    return {{_mm_castps_si128(compareNotEqual(a.components.lo, b.components.lo)),
             _mm_castps_si128(compareNotEqual(a.components.hi, b.components.hi))}};
#endif
  }

  std::ostream &operator<<(std::ostream &stream, const v_f32_8 &v) {
    return stream << "{" << x1(v) << ", " << x2(v) << ", " << x3(v) << ", " << x4(v) << ", " << x5(v) << ", " << x6(v)
                  << ", " << x7(v) << ", " << x8(v) << "}";
  }
}
