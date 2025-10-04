#include "vectorization/v_i32_4.h"

namespace vectorization {
  v_i32_4 operator-(const v_i32_4 &v) noexcept {
    return _mm_sub_epi32(Zero<v_i32_4::PackedType>(), v.components);
  }

  v_i32_4 operator!(const v_i32_4 &v) noexcept {
    return _mm_cmpeq_epi32(Zero<v_i32_4::PackedType>(), v.components);
  }

  v_i32_4 operator~(const v_i32_4 &v) noexcept {
    return _mm_andnot_si128(v.components, MaskAll<v_i32_4::PackedType>());
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
    return _mm_and_si128(a.components, b.components);
  }

  v_i32_4 operator|(const v_i32_4 &a, const v_i32_4 &b) noexcept {
    return _mm_or_si128(a.components, b.components);
  }

  v_i32_4 operator^(const v_i32_4 &a, const v_i32_4 &b) noexcept {
    return _mm_xor_si128(a.components, b.components);
  }

  v_i32_4 operator<<(const v_i32_4 &a, const v_i32_4 &b) noexcept {
    return _mm_sll_epi32(a.components, b.components);
  }

  v_i32_4 operator>>(const v_i32_4 &a, const v_i32_4 &b) noexcept {
    return _mm_srl_epi32(a.components, b.components);
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

  v_i32_4 &operator<<=(v_i32_4 &a, const v_i32_4 &b) noexcept {
    return a = a << b;
  }

  v_i32_4 &operator>>=(v_i32_4 &a, const v_i32_4 &b) noexcept {
    return a = a >> b;
  }
}
