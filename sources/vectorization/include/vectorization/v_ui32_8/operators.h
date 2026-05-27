#pragma once

#include "type.h"

#include "vectorization/v_i32_8/type.h"

#include <ostream>

namespace vectorization {

  v_ui32_8 operator!(const v_ui32_8 &v) noexcept;

  v_ui32_8 operator~(const v_ui32_8 &v) noexcept;

  v_ui32_8 operator+(const v_ui32_8 &a, const v_ui32_8 &b) noexcept;

  v_ui32_8 operator-(const v_ui32_8 &a, const v_ui32_8 &b) noexcept;

  v_ui32_8 operator*(const v_ui32_8 &a, const v_ui32_8 &b) noexcept;

  v_ui32_8 operator&(const v_ui32_8 &a, const v_ui32_8 &b) noexcept;

  v_ui32_8 operator|(const v_ui32_8 &a, const v_ui32_8 &b) noexcept;

  v_ui32_8 operator^(const v_ui32_8 &a, const v_ui32_8 &b) noexcept;

  v_ui32_8 operator<<(const v_ui32_8 &a, Int_32 count) noexcept;
  v_ui32_8 operator<<(const v_ui32_8 &a, UInt_32 count) noexcept;
  v_ui32_8 operator<<(const v_ui32_8 &a, const v_i32_8 &b) noexcept;
  v_ui32_8 operator<<(const v_ui32_8 &a, const v_ui32_8 &b) noexcept;

  v_ui32_8 operator>>(const v_ui32_8 &a, Int_32 count) noexcept;
  v_ui32_8 operator>>(const v_ui32_8 &a, UInt_32 count) noexcept;
  v_ui32_8 operator>>(const v_ui32_8 &a, const v_i32_8 &b) noexcept;
  v_ui32_8 operator>>(const v_ui32_8 &a, const v_ui32_8 &b) noexcept;

  v_ui32_8 operator<(const v_ui32_8 &a, const v_ui32_8 &b) noexcept;

  v_ui32_8 operator>(const v_ui32_8 &a, const v_ui32_8 &b) noexcept;

  v_ui32_8 operator<=(const v_ui32_8 &a, const v_ui32_8 &b) noexcept;

  v_ui32_8 operator>=(const v_ui32_8 &a, const v_ui32_8 &b) noexcept;

  v_ui32_8 operator==(const v_ui32_8 &a, const v_ui32_8 &b) noexcept;

  v_ui32_8 operator!=(const v_ui32_8 &a, const v_ui32_8 &b) noexcept;

  v_ui32_8 &operator+=(v_ui32_8 &a, const v_ui32_8 &b) noexcept;

  v_ui32_8 &operator-=(v_ui32_8 &a, const v_ui32_8 &b) noexcept;

  v_ui32_8 &operator*=(v_ui32_8 &a, const v_ui32_8 &b) noexcept;

  v_ui32_8 &operator&=(v_ui32_8 &a, const v_ui32_8 &b) noexcept;

  v_ui32_8 &operator|=(v_ui32_8 &a, const v_ui32_8 &b) noexcept;

  v_ui32_8 &operator^=(v_ui32_8 &a, const v_ui32_8 &b) noexcept;

  v_ui32_8 &operator<<=(v_ui32_8 &a, Int_32 count) noexcept;
  v_ui32_8 &operator<<=(v_ui32_8 &a, UInt_32 count) noexcept;
  v_ui32_8 &operator<<=(v_ui32_8 &a, const v_i32_8 &b) noexcept;
  v_ui32_8 &operator<<=(v_ui32_8 &a, const v_ui32_8 &b) noexcept;

  v_ui32_8 &operator>>=(v_ui32_8 &a, Int_32 count) noexcept;
  v_ui32_8 &operator>>=(v_ui32_8 &a, UInt_32 count) noexcept;
  v_ui32_8 &operator>>=(v_ui32_8 &a, const v_i32_8 &b) noexcept;
  v_ui32_8 &operator>>=(v_ui32_8 &a, const v_ui32_8 &b) noexcept;

  std::ostream &operator<<(std::ostream &stream, const v_ui32_8 &v);
}
