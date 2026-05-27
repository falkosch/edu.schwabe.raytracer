#pragma once

#include "type.h"

#include "vectorization/v_ui32_8/type.h"

#include <ostream>

namespace vectorization {
  v_f32_8 operator-(const v_f32_8 &v) noexcept;

  v_f32_8 operator~(const v_f32_8 &v) noexcept;

  v_f32_8::VectorBoolType operator!(const v_f32_8 &v) noexcept;

  v_f32_8 operator+(const v_f32_8 &a, const v_f32_8 &b) noexcept;

  v_f32_8 operator+(const v_f32_8 &a, const v_f32_8::ValueType &b) noexcept;

  v_f32_8 operator+(const v_f32_8::ValueType &a, const v_f32_8 &b) noexcept;

  v_f32_8 operator-(const v_f32_8 &a, const v_f32_8 &b) noexcept;

  v_f32_8 operator-(const v_f32_8 &a, const v_f32_8::ValueType &b) noexcept;

  v_f32_8 operator-(const v_f32_8::ValueType &a, const v_f32_8 &b) noexcept;

  v_f32_8 operator*(const v_f32_8 &a, const v_f32_8 &b) noexcept;

  v_f32_8 operator*(const v_f32_8 &a, const v_f32_8::ValueType &b) noexcept;

  v_f32_8 operator*(const v_f32_8::ValueType &a, const v_f32_8 &b) noexcept;

  v_f32_8 operator/(const v_f32_8 &a, const v_f32_8 &b) noexcept;

  v_f32_8 operator/(const v_f32_8 &a, const v_f32_8::ValueType &b) noexcept;

  v_f32_8 operator/(const v_f32_8::ValueType &a, const v_f32_8 &b) noexcept;

  v_f32_8 operator%(const v_f32_8 &a, const v_f32_8 &b) noexcept;

  v_f32_8 operator&(const v_f32_8 &a, const v_f32_8 &b) noexcept;

  v_f32_8 operator|(const v_f32_8 &a, const v_f32_8 &b) noexcept;

  v_f32_8 operator^(const v_f32_8 &a, const v_f32_8 &b) noexcept;

  v_f32_8 operator<<(const v_f32_8 &a, Int_32 count) noexcept;
  v_f32_8 operator<<(const v_f32_8 &a, UInt_32 count) noexcept;
  v_f32_8 operator<<(const v_f32_8 &a, const v_i32_8 &b) noexcept;
  v_f32_8 operator<<(const v_f32_8 &a, const v_ui32_8 &b) noexcept;

  v_f32_8 operator>>(const v_f32_8 &a, Int_32 count) noexcept;
  v_f32_8 operator>>(const v_f32_8 &a, UInt_32 count) noexcept;
  v_f32_8 operator>>(const v_f32_8 &a, const v_i32_8 &b) noexcept;
  v_f32_8 operator>>(const v_f32_8 &a, const v_ui32_8 &b) noexcept;

  v_f32_8 &operator+=(v_f32_8 &a, const v_f32_8 &b) noexcept;

  v_f32_8 &operator-=(v_f32_8 &a, const v_f32_8 &b) noexcept;

  v_f32_8 &operator*=(v_f32_8 &a, const v_f32_8 &b) noexcept;

  v_f32_8 &operator/=(v_f32_8 &a, const v_f32_8 &b) noexcept;

  v_f32_8 &operator%=(v_f32_8 &a, const v_f32_8 &b) noexcept;

  v_f32_8 &operator&=(v_f32_8 &a, const v_f32_8 &b) noexcept;

  v_f32_8 &operator|=(v_f32_8 &a, const v_f32_8 &b) noexcept;

  v_f32_8 &operator^=(v_f32_8 &a, const v_f32_8 &b) noexcept;

  v_f32_8 &operator<<=(v_f32_8 &a, Int_32 count) noexcept;
  v_f32_8 &operator<<=(v_f32_8 &a, UInt_32 count) noexcept;
  v_f32_8 &operator<<=(v_f32_8 &a, const v_i32_8 &b) noexcept;
  v_f32_8 &operator<<=(v_f32_8 &a, const v_ui32_8 &b) noexcept;

  v_f32_8 &operator>>=(v_f32_8 &a, Int_32 count) noexcept;
  v_f32_8 &operator>>=(v_f32_8 &a, UInt_32 count) noexcept;
  v_f32_8 &operator>>=(v_f32_8 &a, const v_i32_8 &b) noexcept;
  v_f32_8 &operator>>=(v_f32_8 &a, const v_ui32_8 &b) noexcept;

  v_f32_8::VectorBoolType operator<(const v_f32_8 &a, const v_f32_8 &b) noexcept;

  v_f32_8::VectorBoolType operator>(const v_f32_8 &a, const v_f32_8 &b) noexcept;

  v_f32_8::VectorBoolType operator<=(const v_f32_8 &a, const v_f32_8 &b) noexcept;

  v_f32_8::VectorBoolType operator>=(const v_f32_8 &a, const v_f32_8 &b) noexcept;

  v_f32_8::VectorBoolType operator==(const v_f32_8 &a, const v_f32_8 &b) noexcept;

  v_f32_8::VectorBoolType operator!=(const v_f32_8 &a, const v_f32_8 &b) noexcept;

  std::ostream &operator<<(std::ostream &stream, const v_f32_8 &v);
}
