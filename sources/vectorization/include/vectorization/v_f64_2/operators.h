#pragma once

#include "type.h"

#include <ostream>

namespace vectorization {
  v_f64_2 operator-(const v_f64_2 &v) noexcept;

  v_f64_2 operator~(const v_f64_2 &v) noexcept;

  v_f64_2::VectorBoolType operator!(const v_f64_2 &v) noexcept;

  v_f64_2 operator+(const v_f64_2 &a, const v_f64_2 &b) noexcept;

  v_f64_2 operator+(const v_f64_2 &a, const v_f64_2::ValueType &b) noexcept;

  v_f64_2 operator+(const v_f64_2::ValueType &a, const v_f64_2 &b) noexcept;

  v_f64_2 operator-(const v_f64_2 &a, const v_f64_2 &b) noexcept;

  v_f64_2 operator-(const v_f64_2 &a, const v_f64_2::ValueType &b) noexcept;

  v_f64_2 operator-(const v_f64_2::ValueType &a, const v_f64_2 &b) noexcept;

  v_f64_2 operator*(const v_f64_2 &a, const v_f64_2 &b) noexcept;

  v_f64_2 operator*(const v_f64_2 &a, const v_f64_2::ValueType &b) noexcept;

  v_f64_2 operator*(const v_f64_2::ValueType &a, const v_f64_2 &b) noexcept;

  v_f64_2 operator/(const v_f64_2 &a, const v_f64_2 &b) noexcept;

  v_f64_2 operator/(const v_f64_2 &a, const v_f64_2::ValueType &b) noexcept;

  v_f64_2 operator/(const v_f64_2::ValueType &a, const v_f64_2 &b) noexcept;

  v_f64_2 operator%(const v_f64_2 &a, const v_f64_2 &b) noexcept;

  v_f64_2 operator&(const v_f64_2 &a, const v_f64_2 &b) noexcept;

  v_f64_2 operator|(const v_f64_2 &a, const v_f64_2 &b) noexcept;

  v_f64_2 operator^(const v_f64_2 &a, const v_f64_2 &b) noexcept;

  v_f64_2 &operator+=(v_f64_2 &a, const v_f64_2 &b) noexcept;

  v_f64_2 &operator-=(v_f64_2 &a, const v_f64_2 &b) noexcept;

  v_f64_2 &operator*=(v_f64_2 &a, const v_f64_2 &b) noexcept;

  v_f64_2 &operator/=(v_f64_2 &a, const v_f64_2 &b) noexcept;

  v_f64_2 &operator%=(v_f64_2 &a, const v_f64_2 &b) noexcept;

  v_f64_2 &operator&=(v_f64_2 &a, const v_f64_2 &b) noexcept;

  v_f64_2 &operator|=(v_f64_2 &a, const v_f64_2 &b) noexcept;

  v_f64_2 &operator^=(v_f64_2 &a, const v_f64_2 &b) noexcept;

  v_f64_2::VectorBoolType operator<(const v_f64_2 &a, const v_f64_2 &b) noexcept;

  v_f64_2::VectorBoolType operator>(const v_f64_2 &a, const v_f64_2 &b) noexcept;

  v_f64_2::VectorBoolType operator<=(const v_f64_2 &a, const v_f64_2 &b) noexcept;

  v_f64_2::VectorBoolType operator>=(const v_f64_2 &a, const v_f64_2 &b) noexcept;

  v_f64_2::VectorBoolType operator==(const v_f64_2 &a, const v_f64_2 &b) noexcept;

  v_f64_2::VectorBoolType operator!=(const v_f64_2 &a, const v_f64_2 &b) noexcept;

  std::ostream &operator<<(std::ostream &stream, const v_f64_2 &v);
}
