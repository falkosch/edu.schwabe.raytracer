#pragma once

#include "type.h"

namespace vectorization {
  v_ui32_4 operator!(const v_ui32_4 &vector) noexcept;

  v_ui32_4 operator~(const v_ui32_4 &vector) noexcept;

  v_ui32_4 operator+(const v_ui32_4 &a, const v_ui32_4 &b) noexcept;

  v_ui32_4 operator-(const v_ui32_4 &a, const v_ui32_4 &b) noexcept;

  v_ui32_4 operator*(const v_ui32_4 &a, const v_ui32_4 &b) noexcept;

  v_ui32_4 operator/(const v_ui32_4 &a, const v_ui32_4 &b) noexcept;

  v_ui32_4 operator%(const v_ui32_4 &a, const v_ui32_4 &b) noexcept;

  v_ui32_4 operator&(const v_ui32_4 &a, const v_ui32_4 &b) noexcept;

  v_ui32_4 operator|(const v_ui32_4 &a, const v_ui32_4 &b) noexcept;

  v_ui32_4 operator^(const v_ui32_4 &a, const v_ui32_4 &b) noexcept;

  v_ui32_4 operator<<(const v_ui32_4 &a, const v_ui32_4 &b) noexcept;

  v_ui32_4 operator>>(const v_ui32_4 &a, const v_ui32_4 &b) noexcept;

  v_ui32_4 operator<(const v_ui32_4 &a, const v_ui32_4 &b) noexcept;

  v_ui32_4 operator>(const v_ui32_4 &a, const v_ui32_4 &b) noexcept;

  v_ui32_4 operator<=(const v_ui32_4 &a, const v_ui32_4 &b) noexcept;

  v_ui32_4 operator>=(const v_ui32_4 &a, const v_ui32_4 &b) noexcept;

  v_ui32_4 operator==(const v_ui32_4 &a, const v_ui32_4 &b) noexcept;

  v_ui32_4 operator!=(const v_ui32_4 &a, const v_ui32_4 &b) noexcept;

  v_ui32_4 &operator+=(v_ui32_4 &a, const v_ui32_4 &b) noexcept;

  v_ui32_4 &operator-=(v_ui32_4 &a, const v_ui32_4 &b) noexcept;

  v_ui32_4 &operator*=(v_ui32_4 &a, const v_ui32_4 &b) noexcept;

  v_ui32_4 &operator/=(v_ui32_4 &a, const v_ui32_4 &b) noexcept;

  v_ui32_4 &operator%=(v_ui32_4 &a, const v_ui32_4 &b) noexcept;

  v_ui32_4 &operator&=(v_ui32_4 &a, const v_ui32_4 &b) noexcept;

  v_ui32_4 &operator|=(v_ui32_4 &a, const v_ui32_4 &b) noexcept;

  v_ui32_4 &operator^=(v_ui32_4 &a, const v_ui32_4 &b) noexcept;

  v_ui32_4 &operator<<=(v_ui32_4 &a, const v_ui32_4 &b) noexcept;

  v_ui32_4 &operator>>=(v_ui32_4 &a, const v_ui32_4 &b) noexcept;
}
