#pragma once

#include "type.h"

namespace vectorization {
  v_i64_2 operator-(const v_i64_2 &vector) noexcept;
  v_i64_2 operator!(const v_i64_2 &vector) noexcept;
  v_i64_2 operator~(const v_i64_2 &vector) noexcept;

  v_i64_2 operator<(const v_i64_2 &a, const v_i64_2 &b) noexcept;
  v_i64_2 operator>(const v_i64_2 &a, const v_i64_2 &b) noexcept;
  v_i64_2 operator<=(const v_i64_2 &a, const v_i64_2 &b) noexcept;
  v_i64_2 operator>=(const v_i64_2 &a, const v_i64_2 &b) noexcept;
  v_i64_2 operator==(const v_i64_2 &a, const v_i64_2 &b) noexcept;
  v_i64_2 operator!=(const v_i64_2 &a, const v_i64_2 &b) noexcept;

  v_i64_2 operator+(const v_i64_2 &a, const v_i64_2 &b) noexcept;
  v_i64_2 operator-(const v_i64_2 &a, const v_i64_2 &b) noexcept;
  v_i64_2 operator*(const v_i64_2 &a, const v_i64_2 &b) noexcept;
  v_i64_2 operator/(const v_i64_2 &a, const v_i64_2 &b) noexcept;
  v_i64_2 operator%(const v_i64_2 &a, const v_i64_2 &b) noexcept;
  v_i64_2 operator&(const v_i64_2 &a, const v_i64_2 &b) noexcept;
  v_i64_2 operator|(const v_i64_2 &a, const v_i64_2 &b) noexcept;
  v_i64_2 operator^(const v_i64_2 &a, const v_i64_2 &b) noexcept;
  v_i64_2 operator<<(const v_i64_2 &a, const v_i64_2 &b) noexcept;
  v_i64_2 operator>>(const v_i64_2 &a, const v_i64_2 &b) noexcept;

  v_i64_2 &operator+=(v_i64_2 &a, const v_i64_2 &b) noexcept;
  v_i64_2 &operator-=(v_i64_2 &a, const v_i64_2 &b) noexcept;
  v_i64_2 &operator*=(v_i64_2 &a, const v_i64_2 &b) noexcept;
  v_i64_2 &operator/=(v_i64_2 &a, const v_i64_2 &b) noexcept;
  v_i64_2 &operator%=(v_i64_2 &a, const v_i64_2 &b) noexcept;
  v_i64_2 &operator&=(v_i64_2 &a, const v_i64_2 &b) noexcept;
  v_i64_2 &operator|=(v_i64_2 &a, const v_i64_2 &b) noexcept;
  v_i64_2 &operator^=(v_i64_2 &a, const v_i64_2 &b) noexcept;
  v_i64_2 &operator<<=(v_i64_2 &a, const v_i64_2 &b) noexcept;
  v_i64_2 &operator>>=(v_i64_2 &a, const v_i64_2 &b) noexcept;
}
