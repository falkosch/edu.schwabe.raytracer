#include "vectorization/v_ui32_4.h"

#include "vectorization/functions/bitwise.h"
#include "vectorization/functions/shift.h"
#include "vectorization/v_i32_4/type.h"

namespace vectorization {
  v_ui32_4 operator!(const v_ui32_4 &vector) noexcept {
    return _mm_cmpeq_epi32(Zero<v_ui32_4::PackedType>(), vector.components);
  }

  v_ui32_4 operator~(const v_ui32_4 &vector) noexcept {
    return bitwiseNot(vector.components);
  }

  v_ui32_4 operator+(const v_ui32_4 &a, const v_ui32_4 &b) noexcept {
    return _mm_add_epi32(a.components, b.components);
  }

  v_ui32_4 operator-(const v_ui32_4 &a, const v_ui32_4 &b) noexcept {
    return _mm_sub_epi32(a.components, b.components);
  }

  v_ui32_4 operator*(const v_ui32_4 &a, const v_ui32_4 &b) noexcept {
    return _mm_mullo_epi32(a.components, b.components);
    // return swizzledBlend<VectorIndices::X, VectorIndices::X, VectorIndices::Z, VectorIndices::Z, false, true, false,
    // true>(
    //     _mm_mul_epu32(a.components, b.components),
    //     _mm_mul_epu32(yxwz(a).components, yxwz(b).components)
    //);
  }

  v_ui32_4 operator/(const v_ui32_4 &a, const v_ui32_4 &b) noexcept {
    return v_ui32_4(x(a) / x(b), y(a) / y(b), z(a) / z(b), w(a) / w(b));
  }

  v_ui32_4 operator%(const v_ui32_4 &a, const v_ui32_4 &b) noexcept {
    return v_ui32_4(x(a) % x(b), y(a) % y(b), z(a) % z(b), w(a) % w(b));
  }

  v_ui32_4 operator&(const v_ui32_4 &a, const v_ui32_4 &b) noexcept {
    return bitwiseAnd(a.components, b.components);
  }

  v_ui32_4 operator|(const v_ui32_4 &a, const v_ui32_4 &b) noexcept {
    return bitwiseOr(a.components, b.components);
  }

  v_ui32_4 operator^(const v_ui32_4 &a, const v_ui32_4 &b) noexcept {
    return bitwiseXor(a.components, b.components);
  }

  v_ui32_4 operator<<(const v_ui32_4 &a, const Int_32 count) noexcept {
    return shiftLeft32(a.components, count);
  }

  v_ui32_4 operator<<(const v_ui32_4 &a, const UInt_32 count) noexcept {
    return shiftLeft32(a.components, count);
  }

  v_ui32_4 operator<<(const v_ui32_4 &a, const v_i32_4 &b) noexcept {
    return shiftLeft32(a.components, b.components);
  }

  v_ui32_4 operator<<(const v_ui32_4 &a, const v_ui32_4 &b) noexcept {
    return shiftLeft32(a.components, b.components);
  }

  v_ui32_4 operator>>(const v_ui32_4 &a, const Int_32 count) noexcept {
    return shiftRightLogical32(a.components, count);
  }

  v_ui32_4 operator>>(const v_ui32_4 &a, const UInt_32 count) noexcept {
    return shiftRightLogical32(a.components, count);
  }

  v_ui32_4 operator>>(const v_ui32_4 &a, const v_i32_4 &b) noexcept {
    return shiftRightLogical32(a.components, b.components);
  }

  v_ui32_4 operator>>(const v_ui32_4 &a, const v_ui32_4 &b) noexcept {
    return shiftRightLogical32(a.components, b.components);
  }

  v_ui32_4 operator<(const v_ui32_4 &a, const v_ui32_4 &b) noexcept {
    return ~(a >= b);
  }

  v_ui32_4 operator>(const v_ui32_4 &a, const v_ui32_4 &b) noexcept {
    return ~(a <= b);
  }

  v_ui32_4 operator<=(const v_ui32_4 &a, const v_ui32_4 &b) noexcept {
    return _mm_cmpeq_epi32(a.components, _mm_min_epu32(a.components, b.components));
  }

  v_ui32_4 operator>=(const v_ui32_4 &a, const v_ui32_4 &b) noexcept {
    return _mm_cmpeq_epi32(a.components, _mm_max_epu32(a.components, b.components));
  }

  v_ui32_4 operator==(const v_ui32_4 &a, const v_ui32_4 &b) noexcept {
    return _mm_cmpeq_epi32(a.components, b.components);
  }

  v_ui32_4 operator!=(const v_ui32_4 &a, const v_ui32_4 &b) noexcept {
    return !(a == b);
  }

  v_ui32_4 &operator+=(v_ui32_4 &a, const v_ui32_4 &b) noexcept {
    return a = a + b;
  }

  v_ui32_4 &operator-=(v_ui32_4 &a, const v_ui32_4 &b) noexcept {
    return a = a - b;
  }

  v_ui32_4 &operator*=(v_ui32_4 &a, const v_ui32_4 &b) noexcept {
    return a = a * b;
  }

  v_ui32_4 &operator/=(v_ui32_4 &a, const v_ui32_4 &b) noexcept {
    return a = a / b;
  }

  v_ui32_4 &operator%=(v_ui32_4 &a, const v_ui32_4 &b) noexcept {
    return a = a % b;
  }

  v_ui32_4 &operator&=(v_ui32_4 &a, const v_ui32_4 &b) noexcept {
    return a = a & b;
  }

  v_ui32_4 &operator|=(v_ui32_4 &a, const v_ui32_4 &b) noexcept {
    return a = a | b;
  }

  v_ui32_4 &operator^=(v_ui32_4 &a, const v_ui32_4 &b) noexcept {
    return a = a ^ b;
  }

  v_ui32_4 &operator<<=(v_ui32_4 &a, const Int_32 count) noexcept { return a = a << count; }
  v_ui32_4 &operator<<=(v_ui32_4 &a, const UInt_32 count) noexcept { return a = a << count; }
  v_ui32_4 &operator<<=(v_ui32_4 &a, const v_i32_4 &b) noexcept { return a = a << b; }
  v_ui32_4 &operator<<=(v_ui32_4 &a, const v_ui32_4 &b) noexcept { return a = a << b; }

  v_ui32_4 &operator>>=(v_ui32_4 &a, const Int_32 count) noexcept { return a = a >> count; }
  v_ui32_4 &operator>>=(v_ui32_4 &a, const UInt_32 count) noexcept { return a = a >> count; }
  v_ui32_4 &operator>>=(v_ui32_4 &a, const v_i32_4 &b) noexcept { return a = a >> b; }
  v_ui32_4 &operator>>=(v_ui32_4 &a, const v_ui32_4 &b) noexcept { return a = a >> b; }
}
