#pragma once

#include "type.h"

#include <ostream>

namespace vectorization {

  v_i32_8 operator-(const v_i32_8 &v) noexcept;

  v_i32_8 operator!(const v_i32_8 &v) noexcept;

  v_i32_8 operator~(const v_i32_8 &v) noexcept;

  v_i32_8 operator+(const v_i32_8 &a, const v_i32_8 &b) noexcept;

  v_i32_8 operator-(const v_i32_8 &a, const v_i32_8 &b) noexcept;

  v_i32_8 operator*(const v_i32_8 &a, const v_i32_8 &b) noexcept;

  v_i32_8 operator&(const v_i32_8 &a, const v_i32_8 &b) noexcept;

  v_i32_8 operator|(const v_i32_8 &a, const v_i32_8 &b) noexcept;

  v_i32_8 operator^(const v_i32_8 &a, const v_i32_8 &b) noexcept;

  v_i32_8 operator<<(const v_i32_8 &a, const v_i32_8 &b) noexcept;

  v_i32_8 operator>>(const v_i32_8 &a, const v_i32_8 &b) noexcept;

  v_i32_8 operator<(const v_i32_8 &a, const v_i32_8 &b) noexcept;

  v_i32_8 operator>(const v_i32_8 &a, const v_i32_8 &b) noexcept;

  v_i32_8 operator==(const v_i32_8 &a, const v_i32_8 &b) noexcept;

  v_i32_8 operator!=(const v_i32_8 &a, const v_i32_8 &b) noexcept;

  v_i32_8 &operator+=(v_i32_8 &a, const v_i32_8 &b) noexcept;

  v_i32_8 &operator-=(v_i32_8 &a, const v_i32_8 &b) noexcept;

  v_i32_8 &operator*=(v_i32_8 &a, const v_i32_8 &b) noexcept;

  v_i32_8 &operator&=(v_i32_8 &a, const v_i32_8 &b) noexcept;

  v_i32_8 &operator|=(v_i32_8 &a, const v_i32_8 &b) noexcept;

  v_i32_8 &operator^=(v_i32_8 &a, const v_i32_8 &b) noexcept;

  v_i32_8 &operator<<=(v_i32_8 &a, const v_i32_8 &b) noexcept;

  v_i32_8 &operator>>=(v_i32_8 &a, const v_i32_8 &b) noexcept;

  std::ostream &operator<<(std::ostream &stream, const v_i32_8 &v);
}
