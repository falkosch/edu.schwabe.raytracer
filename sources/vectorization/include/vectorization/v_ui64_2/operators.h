#pragma once

#include "type.h"

namespace vectorization {
  v_ui64_2 operator-(const v_ui64_2 &vector) noexcept;
  v_ui64_2 operator!(const v_ui64_2 &vector) noexcept;
  v_ui64_2 operator~(const v_ui64_2 &vector) noexcept;

  v_ui64_2 operator<(const v_ui64_2 &a, const v_ui64_2 &b) noexcept;
  v_ui64_2 operator>(const v_ui64_2 &a, const v_ui64_2 &b) noexcept;
  v_ui64_2 operator<=(const v_ui64_2 &a, const v_ui64_2 &b) noexcept;
  v_ui64_2 operator>=(const v_ui64_2 &a, const v_ui64_2 &b) noexcept;
  v_ui64_2 operator==(const v_ui64_2 &a, const v_ui64_2 &b) noexcept;
  v_ui64_2 operator!=(const v_ui64_2 &a, const v_ui64_2 &b) noexcept;

  v_ui64_2 operator+(const v_ui64_2 &a, const v_ui64_2 &b) noexcept;
  v_ui64_2 operator-(const v_ui64_2 &a, const v_ui64_2 &b) noexcept;
  v_ui64_2 operator*(const v_ui64_2 &a, const v_ui64_2 &b) noexcept;
  v_ui64_2 operator/(const v_ui64_2 &a, const v_ui64_2 &b) noexcept;
  v_ui64_2 operator%(const v_ui64_2 &a, const v_ui64_2 &b) noexcept;
  v_ui64_2 operator&(const v_ui64_2 &a, const v_ui64_2 &b) noexcept;
  v_ui64_2 operator|(const v_ui64_2 &a, const v_ui64_2 &b) noexcept;
  v_ui64_2 operator^(const v_ui64_2 &a, const v_ui64_2 &b) noexcept;
  v_ui64_2 operator<<(const v_ui64_2 &a, const v_ui64_2 &b) noexcept;
  v_ui64_2 operator>>(const v_ui64_2 &a, const v_ui64_2 &b) noexcept;
  
  v_ui64_2 &operator+=(v_ui64_2 &a, const v_ui64_2 &b) noexcept;
  v_ui64_2 &operator-=(v_ui64_2 &a, const v_ui64_2 &b) noexcept;
  v_ui64_2 &operator*=(v_ui64_2 &a, const v_ui64_2 &b) noexcept;
  v_ui64_2 &operator/=(v_ui64_2 &a, const v_ui64_2 &b) noexcept;
  v_ui64_2 &operator%=(v_ui64_2 &a, const v_ui64_2 &b) noexcept;
  v_ui64_2 &operator&=(v_ui64_2 &a, const v_ui64_2 &b) noexcept;
  v_ui64_2 &operator|=(v_ui64_2 &a, const v_ui64_2 &b) noexcept;
  v_ui64_2 &operator^=(v_ui64_2 &a, const v_ui64_2 &b) noexcept;
  v_ui64_2 &operator<<=(v_ui64_2 &a, const v_ui64_2 &b) noexcept;
  v_ui64_2 &operator>>=(v_ui64_2 &a, const v_ui64_2 &b) noexcept;
}
