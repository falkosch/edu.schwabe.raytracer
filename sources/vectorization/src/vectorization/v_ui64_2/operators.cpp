#include "vectorization/v_ui64_2.h"

#include "vectorization/functions/bitwise.h"
#include "vectorization/functions/shift.h"
#include "vectorization/v_i64_2/type.h"

namespace vectorization {
  v_ui64_2 operator!(const v_ui64_2 &vector) noexcept {
    return _mm_cmpeq_epi64(Zero<v_ui64_2::PackedType>(), vector.components);
  }

  v_ui64_2 operator~(const v_ui64_2 &vector) noexcept {
    return bitwiseNot(vector.components);
  }

  v_ui64_2 operator+(const v_ui64_2 &a, const v_ui64_2 &b) noexcept {
    return _mm_add_epi64(a.components, b.components);
  }

  v_ui64_2 operator-(const v_ui64_2 &a, const v_ui64_2 &b) noexcept {
    return _mm_sub_epi64(a.components, b.components);
  }

  v_ui64_2 operator*(const v_ui64_2 &a, const v_ui64_2 &b) noexcept {
    return v_ui64_2(x(a) * x(b), y(a) * y(b));
  }

  v_ui64_2 operator/(const v_ui64_2 &a, const v_ui64_2 &b) noexcept {
    return v_ui64_2(x(a) / x(b), y(a) / y(b));
  }

  v_ui64_2 operator%(const v_ui64_2 &a, const v_ui64_2 &b) noexcept {
    return v_ui64_2(x(a) % x(b), y(a) % y(b));
  }

  v_ui64_2 operator&(const v_ui64_2 &a, const v_ui64_2 &b) noexcept {
    return bitwiseAnd(a.components, b.components);
  }

  v_ui64_2 operator|(const v_ui64_2 &a, const v_ui64_2 &b) noexcept {
    return bitwiseOr(a.components, b.components);
  }

  v_ui64_2 operator^(const v_ui64_2 &a, const v_ui64_2 &b) noexcept {
    return bitwiseXor(a.components, b.components);
  }

  v_ui64_2 operator<<(const v_ui64_2 &a, const Int_64 count) noexcept {
    return shiftLeft64(a.components, count);
  }

  v_ui64_2 operator<<(const v_ui64_2 &a, const UInt_64 count) noexcept {
    return shiftLeft64(a.components, count);
  }

  v_ui64_2 operator<<(const v_ui64_2 &a, const v_i64_2 &b) noexcept {
    return shiftLeft64(a.components, b.components);
  }

  v_ui64_2 operator<<(const v_ui64_2 &a, const v_ui64_2 &b) noexcept {
    return shiftLeft64(a.components, b.components);
  }

  v_ui64_2 operator>>(const v_ui64_2 &a, const Int_64 count) noexcept {
    return shiftRightLogical64(a.components, count);
  }

  v_ui64_2 operator>>(const v_ui64_2 &a, const UInt_64 count) noexcept {
    return shiftRightLogical64(a.components, count);
  }

  v_ui64_2 operator>>(const v_ui64_2 &a, const v_i64_2 &b) noexcept {
    return shiftRightLogical64(a.components, b.components);
  }

  v_ui64_2 operator>>(const v_ui64_2 &a, const v_ui64_2 &b) noexcept {
    return shiftRightLogical64(a.components, b.components);
  }

  v_ui64_2 operator<(const v_ui64_2 &a, const v_ui64_2 &b) noexcept {
    return !(a >= b);
  }

  v_ui64_2 operator>(const v_ui64_2 &a, const v_ui64_2 &b) noexcept {
    return _mm_cmpgt_epi64(a.components, b.components);
  }

  v_ui64_2 operator<=(const v_ui64_2 &a, const v_ui64_2 &b) noexcept {
    return !(a > b);
  }

  v_ui64_2 operator>=(const v_ui64_2 &a, const v_ui64_2 &b) noexcept {
    const auto greater = a > b;
    const auto equal = a == b;
    return greater | equal;
  }

  v_ui64_2 operator==(const v_ui64_2 &a, const v_ui64_2 &b) noexcept {
    return _mm_cmpeq_epi64(a.components, b.components);
  }

  v_ui64_2 operator!=(const v_ui64_2 &a, const v_ui64_2 &b) noexcept {
    return !(a == b);
  }

  v_ui64_2 &operator+=(v_ui64_2 &a, const v_ui64_2 &b) noexcept {
    return a = a + b;
  }

  v_ui64_2 &operator-=(v_ui64_2 &a, const v_ui64_2 &b) noexcept {
    return a = a - b;
  }

  v_ui64_2 &operator*=(v_ui64_2 &a, const v_ui64_2 &b) noexcept {
    return a = a * b;
  }

  v_ui64_2 &operator/=(v_ui64_2 &a, const v_ui64_2 &b) noexcept {
    return a = a / b;
  }

  v_ui64_2 &operator%=(v_ui64_2 &a, const v_ui64_2 &b) noexcept {
    return a = a % b;
  }

  v_ui64_2 &operator&=(v_ui64_2 &a, const v_ui64_2 &b) noexcept {
    return a = a & b;
  }

  v_ui64_2 &operator|=(v_ui64_2 &a, const v_ui64_2 &b) noexcept {
    return a = a | b;
  }

  v_ui64_2 &operator^=(v_ui64_2 &a, const v_ui64_2 &b) noexcept {
    return a = a ^ b;
  }

  v_ui64_2 &operator<<=(v_ui64_2 &a, const Int_64 count) noexcept { return a = a << count; }
  v_ui64_2 &operator<<=(v_ui64_2 &a, const UInt_64 count) noexcept { return a = a << count; }
  v_ui64_2 &operator<<=(v_ui64_2 &a, const v_i64_2 &b) noexcept { return a = a << b; }
  v_ui64_2 &operator<<=(v_ui64_2 &a, const v_ui64_2 &b) noexcept { return a = a << b; }

  v_ui64_2 &operator>>=(v_ui64_2 &a, const Int_64 count) noexcept { return a = a >> count; }
  v_ui64_2 &operator>>=(v_ui64_2 &a, const UInt_64 count) noexcept { return a = a >> count; }
  v_ui64_2 &operator>>=(v_ui64_2 &a, const v_i64_2 &b) noexcept { return a = a >> b; }
  v_ui64_2 &operator>>=(v_ui64_2 &a, const v_ui64_2 &b) noexcept { return a = a >> b; }
}
