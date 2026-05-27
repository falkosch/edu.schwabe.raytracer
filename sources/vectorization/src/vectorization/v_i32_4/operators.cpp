#include "vectorization/v_i32_4.h"

#include "vectorization/functions/bitwise.h"
#include "vectorization/functions/shift.h"
#include "vectorization/v_ui32_4/type.h"

namespace vectorization {
  v_i32_4 operator-(const v_i32_4 &v) noexcept {
    return _mm_sub_epi32(Zero<v_i32_4::PackedType>(), v.components);
  }

  v_i32_4 operator!(const v_i32_4 &v) noexcept {
    return _mm_cmpeq_epi32(Zero<v_i32_4::PackedType>(), v.components);
  }

  v_i32_4 operator~(const v_i32_4 &v) noexcept {
    return bitwiseNot(v.components);
  }

  v_i32_4 operator+(const v_i32_4 &a, const v_i32_4 &b) noexcept {
    return _mm_add_epi32(a.components, b.components);
  }

  v_i32_4 operator-(const v_i32_4 &a, const v_i32_4 &b) noexcept {
    return _mm_sub_epi32(a.components, b.components);
  }

  v_i32_4 operator*(const v_i32_4 &a, const v_i32_4 &b) noexcept {
    return _mm_mullo_epi32(a.components, b.components);
  }

  v_i32_4 operator/(const v_i32_4 &a, const v_i32_4 &b) noexcept {
    return v_i32_4(x(a) / x(b), y(a) / y(b), z(a) / z(b), w(a) / w(b));
  }

  v_i32_4 operator%(const v_i32_4 &a, const v_i32_4 &b) noexcept {
    return v_i32_4(x(a) % x(b), y(a) % y(b), z(a) % z(b), w(a) % w(b));
  }

  v_i32_4 operator&(const v_i32_4 &a, const v_i32_4 &b) noexcept {
    return bitwiseAnd(a.components, b.components);
  }

  v_i32_4 operator|(const v_i32_4 &a, const v_i32_4 &b) noexcept {
    return bitwiseOr(a.components, b.components);
  }

  v_i32_4 operator^(const v_i32_4 &a, const v_i32_4 &b) noexcept {
    return bitwiseXor(a.components, b.components);
  }

  v_i32_4 operator<<(const v_i32_4 &a, const Int_32 count) noexcept {
    return shiftLeft32(a.components, count);
  }

  v_i32_4 operator<<(const v_i32_4 &a, const UInt_32 count) noexcept {
    return shiftLeft32(a.components, count);
  }

  v_i32_4 operator<<(const v_i32_4 &a, const v_i32_4 &b) noexcept {
    return shiftLeft32(a.components, b.components);
  }

  v_i32_4 operator<<(const v_i32_4 &a, const v_ui32_4 &b) noexcept {
    return shiftLeft32(a.components, b.components);
  }

  v_i32_4 operator>>(const v_i32_4 &a, const Int_32 count) noexcept {
    return shiftRightArithmetic32(a.components, count);
  }

  v_i32_4 operator>>(const v_i32_4 &a, const UInt_32 count) noexcept {
    return shiftRightArithmetic32(a.components, count);
  }

  v_i32_4 operator>>(const v_i32_4 &a, const v_i32_4 &b) noexcept {
    return shiftRightArithmetic32(a.components, b.components);
  }

  v_i32_4 operator>>(const v_i32_4 &a, const v_ui32_4 &b) noexcept {
    return shiftRightArithmetic32(a.components, b.components);
  }

  v_i32_4 operator<(const v_i32_4 &a, const v_i32_4 &b) noexcept {
    return _mm_cmplt_epi32(a.components, b.components);
  }

  v_i32_4 operator>(const v_i32_4 &a, const v_i32_4 &b) noexcept {
    return _mm_cmpgt_epi32(a.components, b.components);
  }

  v_i32_4 operator<=(const v_i32_4 &a, const v_i32_4 &b) noexcept {
    return _mm_cmpeq_epi32(a.components, _mm_min_epi32(a.components, b.components));
  }

  v_i32_4 operator>=(const v_i32_4 &a, const v_i32_4 &b) noexcept {
    return _mm_cmpeq_epi32(a.components, _mm_max_epi32(a.components, b.components));
  }

  v_i32_4 operator==(const v_i32_4 &a, const v_i32_4 &b) noexcept {
    return _mm_cmpeq_epi32(a.components, b.components);
  }

  v_i32_4 operator!=(const v_i32_4 &a, const v_i32_4 &b) noexcept {
    return !(a == b);
  }

  v_i32_4 &operator+=(v_i32_4 &a, const v_i32_4 &b) noexcept {
    return a = a + b;
  }

  v_i32_4 &operator-=(v_i32_4 &a, const v_i32_4 &b) noexcept {
    return a = a - b;
  }

  v_i32_4 &operator*=(v_i32_4 &a, const v_i32_4 &b) noexcept {
    return a = a * b;
  }

  v_i32_4 &operator/=(v_i32_4 &a, const v_i32_4 &b) noexcept {
    return a = a / b;
  }

  v_i32_4 &operator%=(v_i32_4 &a, const v_i32_4 &b) noexcept {
    return a = a % b;
  }

  v_i32_4 &operator&=(v_i32_4 &a, const v_i32_4 &b) noexcept {
    return a = a & b;
  }

  v_i32_4 &operator|=(v_i32_4 &a, const v_i32_4 &b) noexcept {
    return a = a | b;
  }

  v_i32_4 &operator^=(v_i32_4 &a, const v_i32_4 &b) noexcept {
    return a = a ^ b;
  }

  v_i32_4 &operator<<=(v_i32_4 &a, const Int_32 count) noexcept { return a = a << count; }
  v_i32_4 &operator<<=(v_i32_4 &a, const UInt_32 count) noexcept { return a = a << count; }
  v_i32_4 &operator<<=(v_i32_4 &a, const v_i32_4 &b) noexcept { return a = a << b; }
  v_i32_4 &operator<<=(v_i32_4 &a, const v_ui32_4 &b) noexcept { return a = a << b; }

  v_i32_4 &operator>>=(v_i32_4 &a, const Int_32 count) noexcept { return a = a >> count; }
  v_i32_4 &operator>>=(v_i32_4 &a, const UInt_32 count) noexcept { return a = a >> count; }
  v_i32_4 &operator>>=(v_i32_4 &a, const v_i32_4 &b) noexcept { return a = a >> b; }
  v_i32_4 &operator>>=(v_i32_4 &a, const v_ui32_4 &b) noexcept { return a = a >> b; }
}
