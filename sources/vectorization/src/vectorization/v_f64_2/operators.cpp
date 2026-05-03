#include "vectorization/v_f64_2/operators.h"

#include "vectorization/v_f64_2/accessors.h"

#include "vectorization/constants/masks.h"
#include "vectorization/constants/values.h"
#include "vectorization/functions/add.h"
#include "vectorization/functions/broadcast.h"
#include "vectorization/functions/divide.h"
#include "vectorization/functions/modulo.h"
#include "vectorization/functions/multiply.h"
#include "vectorization/functions/subtract.h"

namespace vectorization {
  v_f64_2 operator-(const v_f64_2 &v) noexcept {
    return _mm_xor_pd(NegativeZero<v_f64_2::PackedType>(), v.components);
  }

  v_f64_2 operator~(const v_f64_2 &v) noexcept {
    return _mm_xor_pd(MaskAll<v_f64_2::PackedType>(), v.components);
  }

  v_f64_2::VectorBoolType operator!(const v_f64_2 &v) noexcept {
    return _mm_castpd_si128(_mm_cmpeq_pd(v.components, Zero<v_f64_2::PackedType>()));
  }

  v_f64_2 operator+(const v_f64_2 &a, const v_f64_2 &b) noexcept {
    return add(a.components, b.components);
  }

  v_f64_2 operator+(const v_f64_2 &a, const v_f64_2::ValueType &b) noexcept {
    return add(a.components, broadcast(b));
  }

  v_f64_2 operator+(const v_f64_2::ValueType &a, const v_f64_2 &b) noexcept {
    return add(broadcast(a), b.components);
  }

  v_f64_2 operator-(const v_f64_2 &a, const v_f64_2 &b) noexcept {
    return subtract(a.components, b.components);
  }

  v_f64_2 operator-(const v_f64_2 &a, const v_f64_2::ValueType &b) noexcept {
    return subtract(a.components, broadcast(b));
  }

  v_f64_2 operator-(const v_f64_2::ValueType &a, const v_f64_2 &b) noexcept {
    return subtract(broadcast(a), b.components);
  }

  v_f64_2 operator*(const v_f64_2 &a, const v_f64_2 &b) noexcept {
    return multiply(a.components, b.components);
  }

  v_f64_2 operator*(const v_f64_2 &a, const v_f64_2::ValueType &b) noexcept {
    return multiply(a.components, broadcast(b));
  }

  v_f64_2 operator*(const v_f64_2::ValueType &a, const v_f64_2 &b) noexcept {
    return multiply(broadcast(a), b.components);
  }

  v_f64_2 operator/(const v_f64_2 &a, const v_f64_2 &b) noexcept {
    return divide(a.components, b.components);
  }

  v_f64_2 operator/(const v_f64_2 &a, const v_f64_2::ValueType &b) noexcept {
    return divide(a.components, broadcast(b));
  }

  v_f64_2 operator/(const v_f64_2::ValueType &a, const v_f64_2 &b) noexcept {
    return divide(broadcast(a), b.components);
  }

  v_f64_2 operator%(const v_f64_2 &a, const v_f64_2 &b) noexcept {
    return modulo(a.components, b.components);
  }

  v_f64_2 operator&(const v_f64_2 &a, const v_f64_2 &b) noexcept {
    return _mm_and_pd(a.components, b.components);
  }

  v_f64_2 operator|(const v_f64_2 &a, const v_f64_2 &b) noexcept {
    return _mm_or_pd(a.components, b.components);
  }

  v_f64_2 operator^(const v_f64_2 &a, const v_f64_2 &b) noexcept {
    return _mm_xor_pd(a.components, b.components);
  }

  v_f64_2 &operator+=(v_f64_2 &a, const v_f64_2 &b) noexcept {
    return a = a + b;
  }

  v_f64_2 &operator-=(v_f64_2 &a, const v_f64_2 &b) noexcept {
    return a = a - b;
  }

  v_f64_2 &operator*=(v_f64_2 &a, const v_f64_2 &b) noexcept {
    return a = a * b;
  }

  v_f64_2 &operator/=(v_f64_2 &a, const v_f64_2 &b) noexcept {
    return a = a / b;
  }

  v_f64_2 &operator%=(v_f64_2 &a, const v_f64_2 &b) noexcept {
    return a = a % b;
  }

  v_f64_2 &operator&=(v_f64_2 &a, const v_f64_2 &b) noexcept {
    return a = a & b;
  }

  v_f64_2 &operator|=(v_f64_2 &a, const v_f64_2 &b) noexcept {
    return a = a | b;
  }

  v_f64_2 &operator^=(v_f64_2 &a, const v_f64_2 &b) noexcept {
    return a = a ^ b;
  }

  v_f64_2::VectorBoolType operator<(const v_f64_2 &a, const v_f64_2 &b) noexcept {
    return _mm_castpd_si128(_mm_cmplt_pd(a.components, b.components));
  }

  v_f64_2::VectorBoolType operator>(const v_f64_2 &a, const v_f64_2 &b) noexcept {
    return _mm_castpd_si128(_mm_cmpgt_pd(a.components, b.components));
  }

  v_f64_2::VectorBoolType operator<=(const v_f64_2 &a, const v_f64_2 &b) noexcept {
    return _mm_castpd_si128(_mm_cmple_pd(a.components, b.components));
  }

  v_f64_2::VectorBoolType operator>=(const v_f64_2 &a, const v_f64_2 &b) noexcept {
    return _mm_castpd_si128(_mm_cmpge_pd(a.components, b.components));
  }

  v_f64_2::VectorBoolType operator==(const v_f64_2 &a, const v_f64_2 &b) noexcept {
    return _mm_castpd_si128(_mm_cmpeq_pd(a.components, b.components));
  }

  v_f64_2::VectorBoolType operator!=(const v_f64_2 &a, const v_f64_2 &b) noexcept {
    return _mm_castpd_si128(_mm_cmpneq_pd(a.components, b.components));
  }

  std::ostream &operator<<(std::ostream &stream, const v_f64_2 &v) {
    return stream << "{" << x(v) << ", " << y(v) << "}";
  }
}
