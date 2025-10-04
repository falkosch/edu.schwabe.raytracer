#pragma once

#include "type.h"

namespace vectorization {

  v_i32_4 operator-(const v_i32_4 &v) noexcept;

  v_i32_4 operator!(const v_i32_4 &v) noexcept;

  v_i32_4 operator~(const v_i32_4 &v) noexcept;

  v_i32_4 operator+(const v_i32_4 &a, const v_i32_4 &b) noexcept;

  v_i32_4 operator-(const v_i32_4 &a, const v_i32_4 &b) noexcept;

  v_i32_4 operator*(const v_i32_4 &a, const v_i32_4 &b) noexcept;

  v_i32_4 operator/(const v_i32_4 &a, const v_i32_4 &b) noexcept;

  v_i32_4 operator%(const v_i32_4 &a, const v_i32_4 &b) noexcept;

  v_i32_4 operator&(const v_i32_4 &a, const v_i32_4 &b) noexcept;

  v_i32_4 operator|(const v_i32_4 &a, const v_i32_4 &b) noexcept;

  v_i32_4 operator^(const v_i32_4 &a, const v_i32_4 &b) noexcept;

  v_i32_4 operator<<(const v_i32_4 &a, const v_i32_4 &b) noexcept;

  v_i32_4 operator>>(const v_i32_4 &a, const v_i32_4 &b) noexcept;

  v_i32_4 operator<(const v_i32_4 &a, const v_i32_4 &b) noexcept;

  v_i32_4 operator>(const v_i32_4 &a, const v_i32_4 &b) noexcept;

  v_i32_4 operator<=(const v_i32_4 &a, const v_i32_4 &b) noexcept;

  v_i32_4 operator>=(const v_i32_4 &a, const v_i32_4 &b) noexcept;

  v_i32_4 operator==(const v_i32_4 &a, const v_i32_4 &b) noexcept;

  v_i32_4 operator!=(const v_i32_4 &a, const v_i32_4 &b) noexcept;

  v_i32_4 &operator+=(v_i32_4 &a, const v_i32_4 &b) noexcept;

  v_i32_4 &operator-=(v_i32_4 &a, const v_i32_4 &b) noexcept;

  v_i32_4 &operator*=(v_i32_4 &a, const v_i32_4 &b) noexcept;

  v_i32_4 &operator/=(v_i32_4 &a, const v_i32_4 &b) noexcept;

  v_i32_4 &operator%=(v_i32_4 &a, const v_i32_4 &b) noexcept;

  v_i32_4 &operator&=(v_i32_4 &a, const v_i32_4 &b) noexcept;

  v_i32_4 &operator|=(v_i32_4 &a, const v_i32_4 &b) noexcept;

  v_i32_4 &operator^=(v_i32_4 &a, const v_i32_4 &b) noexcept;

  v_i32_4 &operator<<=(v_i32_4 &a, const v_i32_4 &b) noexcept;

  v_i32_4 &operator>>=(v_i32_4 &a, const v_i32_4 &b) noexcept;
}
