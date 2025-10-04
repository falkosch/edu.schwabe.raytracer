#include "vectorization/v_f32_4/operators.h"

#include "vectorization/v_f32_4/accessors.h"

#include "vectorization/constants.h"
#include "vectorization/functions.h"

namespace vectorization {
  // http://fastcpp.blogspot.de/2011/03/changing-sign-of-float-values-using-sse.html
  v_f32_4 operator-(const v_f32_4 &v) noexcept {
    return _mm_xor_ps(NegativeZero<v_f32_4::PackedType>(), v.components);
  }

  v_f32_4 operator~(const v_f32_4 &v) noexcept {
    return _mm_xor_ps(MaskAll<v_f32_4::PackedType>(), v.components);
  }

  v_f32_4::VectorBoolType operator!(const v_f32_4 &v) noexcept {
    return _mm_cmpeq_epi32(Zero<v_f32_4::VectorBoolType::PackedType>(), _mm_castps_si128(v.components));
  }

  v_f32_4 operator+(const v_f32_4 &a, const v_f32_4 &b) noexcept {
    return _mm_add_ps(a.components, b.components);
  }

  v_f32_4 operator+(const v_f32_4 &a, const v_f32_4::ValueType &b) noexcept {
    return _mm_add_ps(a.components, _mm_set_ps1(b));
  }

  v_f32_4 operator+(const v_f32_4::ValueType &a, const v_f32_4 &b) noexcept {
    return _mm_add_ps(_mm_set_ps1(a), b.components);
  }

  v_f32_4 operator-(const v_f32_4 &a, const v_f32_4 &b) noexcept {
    return _mm_sub_ps(a.components, b.components);
  }

  v_f32_4 operator-(const v_f32_4 &a, const v_f32_4::ValueType &b) noexcept {
    return _mm_sub_ps(a.components, _mm_set_ps1(b));
  }

  v_f32_4 operator-(const v_f32_4::ValueType &a, const v_f32_4 &b) noexcept {
    return _mm_sub_ps(_mm_set_ps1(a), b.components);
  }

  v_f32_4 operator*(const v_f32_4 &a, const v_f32_4 &b) noexcept {
    return _mm_mul_ps(a.components, b.components);
  }

  v_f32_4 operator*(const v_f32_4 &a, const v_f32_4::ValueType &b) noexcept {
    return _mm_mul_ps(a.components, _mm_set_ps1(b));
  }

  v_f32_4 operator*(const v_f32_4::ValueType &a, const v_f32_4 &b) noexcept {
    return _mm_mul_ps(_mm_set_ps1(a), b.components);
  }

  v_f32_4 operator/(const v_f32_4 &a, const v_f32_4 &b) noexcept {
    return divide(a.components, b.components);
  }

  v_f32_4 operator/(const v_f32_4 &a, const v_f32_4::ValueType &b) noexcept {
    return a / v_f32_4(b);
  }

  v_f32_4 operator/(const v_f32_4::ValueType &a, const v_f32_4 &b) noexcept {
    return v_f32_4(a) / b;
  }

  v_f32_4 operator%(const v_f32_4 &a, const v_f32_4 &b) noexcept {
    return modulo(a.components, b.components);
  }

  v_f32_4 operator&(const v_f32_4 &a, const v_f32_4 &b) noexcept {
    return _mm_and_ps(a.components, b.components);
  }

  v_f32_4 operator|(const v_f32_4 &a, const v_f32_4 &b) noexcept {
    return _mm_or_ps(a.components, b.components);
  }

  v_f32_4 operator^(const v_f32_4 &a, const v_f32_4 &b) noexcept {
    return _mm_xor_ps(a.components, b.components);
  }

  v_f32_4 operator<<(const v_f32_4 &a, const v_f32_4::VectorBoolType &b) noexcept {
    return _mm_castsi128_ps(_mm_sll_epi32(_mm_castps_si128(a.components), b.components));
  }

  v_f32_4 operator>>(const v_f32_4 &a, const v_f32_4::VectorBoolType &b) noexcept {
    return _mm_castsi128_ps(_mm_srl_epi32(_mm_castps_si128(a.components), b.components));
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

  v_f32_4 &operator<<=(v_f32_4 &a, const v_f32_4::VectorBoolType &b) noexcept {
    return a = a << b;
  }

  v_f32_4 &operator>>=(v_f32_4 &a, const v_f32_4::VectorBoolType &b) noexcept {
    return a = a >> b;
  }

  v_f32_4::VectorBoolType operator<(const v_f32_4 &a, const v_f32_4 &b) noexcept {
    return _mm_castps_si128(_mm_cmplt_ps(a.components, b.components));
  }

  v_f32_4::VectorBoolType operator>(const v_f32_4 &a, const v_f32_4 &b) noexcept {
    return _mm_castps_si128(_mm_cmpgt_ps(a.components, b.components));
  }

  v_f32_4::VectorBoolType operator<=(const v_f32_4 &a, const v_f32_4 &b) noexcept {
    return _mm_castps_si128(_mm_cmple_ps(a.components, b.components));
  }

  v_f32_4::VectorBoolType operator>=(const v_f32_4 &a, const v_f32_4 &b) noexcept {
    return _mm_castps_si128(_mm_cmpge_ps(a.components, b.components));
  }

  v_f32_4::VectorBoolType operator==(const v_f32_4 &a, const v_f32_4 &b) noexcept {
    return _mm_castps_si128(_mm_cmpeq_ps(a.components, b.components));
  }

  v_f32_4::VectorBoolType operator!=(const v_f32_4 &a, const v_f32_4 &b) noexcept {
    return _mm_castps_si128(_mm_cmpneq_ps(a.components, b.components));
  }

  std::ostream &operator<<(std::ostream &stream, const v_f32_4 &v) {
    return stream << "{" << x(v) << ", " << y(v) << ", " << z(v) << ", " << w(v) << "}";
  }
}
