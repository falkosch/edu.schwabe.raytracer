#pragma once

#include "type.h"

#include "vectorization/v_i32_4/type.h"

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

  v_ui32_4 operator<<(const v_ui32_4 &a, Int_32 count) noexcept;
  v_ui32_4 operator<<(const v_ui32_4 &a, UInt_32 count) noexcept;
  v_ui32_4 operator<<(const v_ui32_4 &a, const v_i32_4 &b) noexcept;
  v_ui32_4 operator<<(const v_ui32_4 &a, const v_ui32_4 &b) noexcept;

  v_ui32_4 operator>>(const v_ui32_4 &a, Int_32 count) noexcept;
  v_ui32_4 operator>>(const v_ui32_4 &a, UInt_32 count) noexcept;
  v_ui32_4 operator>>(const v_ui32_4 &a, const v_i32_4 &b) noexcept;
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

  v_ui32_4 &operator<<=(v_ui32_4 &a, Int_32 count) noexcept;
  v_ui32_4 &operator<<=(v_ui32_4 &a, UInt_32 count) noexcept;
  v_ui32_4 &operator<<=(v_ui32_4 &a, const v_i32_4 &b) noexcept;
  v_ui32_4 &operator<<=(v_ui32_4 &a, const v_ui32_4 &b) noexcept;

  v_ui32_4 &operator>>=(v_ui32_4 &a, Int_32 count) noexcept;
  v_ui32_4 &operator>>=(v_ui32_4 &a, UInt_32 count) noexcept;
  v_ui32_4 &operator>>=(v_ui32_4 &a, const v_i32_4 &b) noexcept;
  v_ui32_4 &operator>>=(v_ui32_4 &a, const v_ui32_4 &b) noexcept;
}
