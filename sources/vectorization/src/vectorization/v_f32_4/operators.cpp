#include "vectorization/v_f32_4/operators.h"

#include "vectorization/v_f32_4/accessors.h"

#include "vectorization/constants/values.h"
#include "vectorization/functions/add.h"
#include "vectorization/functions/bitwise.h"
#include "vectorization/functions/broadcast.h"
#include "vectorization/functions/compare.h"
#include "vectorization/functions/divide.h"
#include "vectorization/functions/modulo.h"
#include "vectorization/functions/multiply.h"
#include "vectorization/functions/negate.h"
#include "vectorization/functions/shift.h"
#include "vectorization/functions/subtract.h"
#include "vectorization/v_ui32_4/type.h"

namespace vectorization {
  v_f32_4 operator-(const v_f32_4 &v) noexcept {
    return negate(v.components);
  }

  v_f32_4 operator~(const v_f32_4 &v) noexcept {
    return bitwiseNot(v.components);
  }

  v_f32_4::VectorBoolType operator!(const v_f32_4 &v) noexcept {
    return _mm_castps_si128(compareEqual(v.components, Zero<v_f32_4::PackedType>()));
  }

  v_f32_4 operator+(const v_f32_4 &a, const v_f32_4 &b) noexcept {
    return add(a.components, b.components);
  }

  v_f32_4 operator+(const v_f32_4 &a, const v_f32_4::ValueType &b) noexcept {
    return add(a.components, broadcast(b));
  }

  v_f32_4 operator+(const v_f32_4::ValueType &a, const v_f32_4 &b) noexcept {
    return add(broadcast(a), b.components);
  }

  v_f32_4 operator-(const v_f32_4 &a, const v_f32_4 &b) noexcept {
    return subtract(a.components, b.components);
  }

  v_f32_4 operator-(const v_f32_4 &a, const v_f32_4::ValueType &b) noexcept {
    return subtract(a.components, broadcast(b));
  }

  v_f32_4 operator-(const v_f32_4::ValueType &a, const v_f32_4 &b) noexcept {
    return subtract(broadcast(a), b.components);
  }

  v_f32_4 operator*(const v_f32_4 &a, const v_f32_4 &b) noexcept {
    return multiply(a.components, b.components);
  }

  v_f32_4 operator*(const v_f32_4 &a, const v_f32_4::ValueType &b) noexcept {
    return multiply(a.components, broadcast(b));
  }

  v_f32_4 operator*(const v_f32_4::ValueType &a, const v_f32_4 &b) noexcept {
    return multiply(broadcast(a), b.components);
  }

  v_f32_4 operator/(const v_f32_4 &a, const v_f32_4 &b) noexcept {
    return divide(a.components, b.components);
  }

  v_f32_4 operator/(const v_f32_4 &a, const v_f32_4::ValueType &b) noexcept {
    return divide(a.components, broadcast(b));
  }

  v_f32_4 operator/(const v_f32_4::ValueType &a, const v_f32_4 &b) noexcept {
    return divide(broadcast(a), b.components);
  }

  v_f32_4 operator%(const v_f32_4 &a, const v_f32_4 &b) noexcept {
    return modulo(a.components, b.components);
  }

  v_f32_4 operator&(const v_f32_4 &a, const v_f32_4 &b) noexcept {
    return bitwiseAnd(a.components, b.components);
  }

  v_f32_4 operator|(const v_f32_4 &a, const v_f32_4 &b) noexcept {
    return bitwiseOr(a.components, b.components);
  }

  v_f32_4 operator^(const v_f32_4 &a, const v_f32_4 &b) noexcept {
    return bitwiseXor(a.components, b.components);
  }

  v_f32_4 operator<<(const v_f32_4 &a, const Int_32 count) noexcept {
    return _mm_castsi128_ps(shiftLeft32(_mm_castps_si128(a.components), count));
  }

  v_f32_4 operator<<(const v_f32_4 &a, const UInt_32 count) noexcept {
    return _mm_castsi128_ps(shiftLeft32(_mm_castps_si128(a.components), count));
  }

  v_f32_4 operator<<(const v_f32_4 &a, const v_i32_4 &b) noexcept {
    return _mm_castsi128_ps(shiftLeft32(_mm_castps_si128(a.components), b.components));
  }

  v_f32_4 operator<<(const v_f32_4 &a, const v_ui32_4 &b) noexcept {
    return _mm_castsi128_ps(shiftLeft32(_mm_castps_si128(a.components), b.components));
  }

  v_f32_4 operator>>(const v_f32_4 &a, const Int_32 count) noexcept {
    return _mm_castsi128_ps(shiftRightLogical32(_mm_castps_si128(a.components), count));
  }

  v_f32_4 operator>>(const v_f32_4 &a, const UInt_32 count) noexcept {
    return _mm_castsi128_ps(shiftRightLogical32(_mm_castps_si128(a.components), count));
  }

  v_f32_4 operator>>(const v_f32_4 &a, const v_i32_4 &b) noexcept {
    return _mm_castsi128_ps(shiftRightLogical32(_mm_castps_si128(a.components), b.components));
  }

  v_f32_4 operator>>(const v_f32_4 &a, const v_ui32_4 &b) noexcept {
    return _mm_castsi128_ps(shiftRightLogical32(_mm_castps_si128(a.components), b.components));
  }

  v_f32_4 &operator+=(v_f32_4 &a, const v_f32_4 &b) noexcept {
    return a = a + b;
  }

  v_f32_4 &operator-=(v_f32_4 &a, const v_f32_4 &b) noexcept {
    return a = a - b;
  }

  v_f32_4 &operator*=(v_f32_4 &a, const v_f32_4 &b) noexcept {
    return a = a * b;
  }

  v_f32_4 &operator/=(v_f32_4 &a, const v_f32_4 &b) noexcept {
    return a = a / b;
  }

  v_f32_4 &operator%=(v_f32_4 &a, const v_f32_4 &b) noexcept {
    return a = a % b;
  }

  v_f32_4 &operator&=(v_f32_4 &a, const v_f32_4 &b) noexcept {
    return a = a & b;
  }

  v_f32_4 &operator|=(v_f32_4 &a, const v_f32_4 &b) noexcept {
    return a = a | b;
  }

  v_f32_4 &operator^=(v_f32_4 &a, const v_f32_4 &b) noexcept {
    return a = a ^ b;
  }

  v_f32_4 &operator<<=(v_f32_4 &a, const Int_32 count) noexcept { return a = a << count; }
  v_f32_4 &operator<<=(v_f32_4 &a, const UInt_32 count) noexcept { return a = a << count; }
  v_f32_4 &operator<<=(v_f32_4 &a, const v_i32_4 &b) noexcept { return a = a << b; }
  v_f32_4 &operator<<=(v_f32_4 &a, const v_ui32_4 &b) noexcept { return a = a << b; }

  v_f32_4 &operator>>=(v_f32_4 &a, const Int_32 count) noexcept { return a = a >> count; }
  v_f32_4 &operator>>=(v_f32_4 &a, const UInt_32 count) noexcept { return a = a >> count; }
  v_f32_4 &operator>>=(v_f32_4 &a, const v_i32_4 &b) noexcept { return a = a >> b; }
  v_f32_4 &operator>>=(v_f32_4 &a, const v_ui32_4 &b) noexcept { return a = a >> b; }

  v_f32_4::VectorBoolType operator<(const v_f32_4 &a, const v_f32_4 &b) noexcept {
    return _mm_castps_si128(compareLess(a.components, b.components));
  }

  v_f32_4::VectorBoolType operator>(const v_f32_4 &a, const v_f32_4 &b) noexcept {
    return _mm_castps_si128(compareGreater(a.components, b.components));
  }

  v_f32_4::VectorBoolType operator<=(const v_f32_4 &a, const v_f32_4 &b) noexcept {
    return _mm_castps_si128(compareLessEqual(a.components, b.components));
  }

  v_f32_4::VectorBoolType operator>=(const v_f32_4 &a, const v_f32_4 &b) noexcept {
    return _mm_castps_si128(compareGreaterEqual(a.components, b.components));
  }

  v_f32_4::VectorBoolType operator==(const v_f32_4 &a, const v_f32_4 &b) noexcept {
    return _mm_castps_si128(compareEqual(a.components, b.components));
  }

  v_f32_4::VectorBoolType operator!=(const v_f32_4 &a, const v_f32_4 &b) noexcept {
    return _mm_castps_si128(compareNotEqual(a.components, b.components));
  }

  std::ostream &operator<<(std::ostream &stream, const v_f32_4 &v) {
    return stream << "{" << x(v) << ", " << y(v) << ", " << z(v) << ", " << w(v) << "}";
  }
}
