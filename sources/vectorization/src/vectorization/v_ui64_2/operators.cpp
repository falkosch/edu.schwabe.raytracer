#include "vectorization/v_ui64_2.h"

namespace vectorization {
  v_ui64_2 operator!(const v_ui64_2 &vector) noexcept {
    return _mm_cmpeq_epi64(Zero<v_ui64_2::PackedType>(), vector.components);
  }

  v_ui64_2 operator~(const v_ui64_2 &vector) noexcept {
    return _mm_andnot_si128(vector.components, MaskAll<v_ui64_2::PackedType>());
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
    return _mm_and_si128(a.components, b.components);
  }

  v_ui64_2 operator|(const v_ui64_2 &a, const v_ui64_2 &b) noexcept {
    return _mm_or_si128(a.components, b.components);
  }

  v_ui64_2 operator^(const v_ui64_2 &a, const v_ui64_2 &b) noexcept {
    return _mm_xor_si128(a.components, b.components);
  }

  v_ui64_2 operator<<(const v_ui64_2 &a, const v_ui64_2 &b) noexcept {
    return _mm_sll_epi64(a.components, b.components);
  }

  v_ui64_2 operator>>(const v_ui64_2 &a, const v_ui64_2 &b) noexcept {
    return _mm_srl_epi64(a.components, b.components);
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
    return (a > b) | (a == b);
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

  v_ui64_2 &operator<<=(v_ui64_2 &a, const v_ui64_2 &b) noexcept {
    return a = a << b;
  }

  v_ui64_2 &operator>>=(v_ui64_2 &a, const v_ui64_2 &b) noexcept {
    return a = a >> b;
  }
}
