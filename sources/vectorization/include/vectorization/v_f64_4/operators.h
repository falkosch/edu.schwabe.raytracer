#pragma once

#include "type.h"

#include <ostream>

namespace vectorization {
  v_f64_4 operator-(const v_f64_4 &v) noexcept;

  v_f64_4 operator~(const v_f64_4 &v) noexcept;

  v_f64_4::VectorBoolType operator!(const v_f64_4 &v) noexcept;

  v_f64_4 operator+(const v_f64_4 &a, const v_f64_4 &b) noexcept;

  v_f64_4 operator+(const v_f64_4 &a, const v_f64_4::ValueType &b) noexcept;

  v_f64_4 operator+(const v_f64_4::ValueType &a, const v_f64_4 &b) noexcept;

  v_f64_4 operator-(const v_f64_4 &a, const v_f64_4 &b) noexcept;

  v_f64_4 operator-(const v_f64_4 &a, const v_f64_4::ValueType &b) noexcept;

  v_f64_4 operator-(const v_f64_4::ValueType &a, const v_f64_4 &b) noexcept;

  v_f64_4 operator*(const v_f64_4 &a, const v_f64_4 &b) noexcept;

  v_f64_4 operator*(const v_f64_4 &a, const v_f64_4::ValueType &b) noexcept;

  v_f64_4 operator*(const v_f64_4::ValueType &a, const v_f64_4 &b) noexcept;

  v_f64_4 operator/(const v_f64_4 &a, const v_f64_4 &b) noexcept;

  v_f64_4 operator/(const v_f64_4 &a, const v_f64_4::ValueType &b) noexcept;

  v_f64_4 operator/(const v_f64_4::ValueType &a, const v_f64_4 &b) noexcept;

  v_f64_4 operator%(const v_f64_4 &a, const v_f64_4 &b) noexcept;

  v_f64_4 operator&(const v_f64_4 &a, const v_f64_4 &b) noexcept;

  v_f64_4 operator|(const v_f64_4 &a, const v_f64_4 &b) noexcept;

  v_f64_4 operator^(const v_f64_4 &a, const v_f64_4 &b) noexcept;

  v_f64_4 operator<<(const v_f64_4 &a, const v_f64_4::VectorBoolType &b) noexcept;

  v_f64_4 operator>>(const v_f64_4 &a, const v_f64_4::VectorBoolType &b) noexcept;

  v_f64_4 &operator+=(v_f64_4 &a, const v_f64_4 &b) noexcept;

  v_f64_4 &operator-=(v_f64_4 &a, const v_f64_4 &b) noexcept;

  v_f64_4 &operator*=(v_f64_4 &a, const v_f64_4 &b) noexcept;

  v_f64_4 &operator/=(v_f64_4 &a, const v_f64_4 &b) noexcept;

  v_f64_4 &operator%=(v_f64_4 &a, const v_f64_4 &b) noexcept;

  v_f64_4 &operator&=(v_f64_4 &a, const v_f64_4 &b) noexcept;

  v_f64_4 &operator|=(v_f64_4 &a, const v_f64_4 &b) noexcept;

  v_f64_4 &operator^=(v_f64_4 &a, const v_f64_4 &b) noexcept;

  v_f64_4 &operator<<=(v_f64_4 &a, const v_f64_4::VectorBoolType &b) noexcept;

  v_f64_4 &operator>>=(v_f64_4 &a, const v_f64_4::VectorBoolType &b) noexcept;

  v_f64_4::VectorBoolType operator<(const v_f64_4 &a, const v_f64_4 &b) noexcept;

  v_f64_4::VectorBoolType operator>(const v_f64_4 &a, const v_f64_4 &b) noexcept;

  v_f64_4::VectorBoolType operator<=(const v_f64_4 &a, const v_f64_4 &b) noexcept;

  v_f64_4::VectorBoolType operator>=(const v_f64_4 &a, const v_f64_4 &b) noexcept;

  v_f64_4::VectorBoolType operator==(const v_f64_4 &a, const v_f64_4 &b) noexcept;

  v_f64_4::VectorBoolType operator!=(const v_f64_4 &a, const v_f64_4 &b) noexcept;

  std::ostream &operator<<(std::ostream &stream, const v_f64_4 &v);
}
