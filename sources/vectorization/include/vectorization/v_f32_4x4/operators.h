#pragma once

#include "type.h"

namespace vectorization {
  v_f32_4x4 operator+(const v_f32_4x4 &a, const v_f32_4x4 &b) noexcept;

  v_f32_4x4 operator-(const v_f32_4x4 &a, const v_f32_4x4 &b) noexcept;

  v_f32_4x4 operator*(const v_f32_4x4 &a, const v_f32_4x4 &b) noexcept;

  v_f32_4x4 operator/(const v_f32_4x4 &a, const v_f32_4x4 &b) noexcept;

  v_f32_4x4 operator%(const v_f32_4x4 &a, const v_f32_4x4 &b) noexcept;

  v_f32_4x4 operator&(const v_f32_4x4 &a, const v_f32_4x4 &b) noexcept;

  v_f32_4x4 operator|(const v_f32_4x4 &a, const v_f32_4x4 &b) noexcept;

  v_f32_4x4 operator^(const v_f32_4x4 &a, const v_f32_4x4 &b) noexcept;

  v_f32_4x4 operator+=(v_f32_4x4 &a, const v_f32_4x4 &b) noexcept;

  v_f32_4x4 operator-=(v_f32_4x4 &a, const v_f32_4x4 &b) noexcept;

  v_f32_4x4 operator*=(v_f32_4x4 &a, const v_f32_4x4 &b) noexcept;

  v_f32_4x4 operator/=(v_f32_4x4 &a, const v_f32_4x4 &b) noexcept;

  v_f32_4x4 operator%=(v_f32_4x4 &a, const v_f32_4x4 &b) noexcept;

  v_f32_4x4 operator&=(v_f32_4x4 &a, const v_f32_4x4 &b) noexcept;

  v_f32_4x4 operator|=(v_f32_4x4 &a, const v_f32_4x4 &b) noexcept;

  v_f32_4x4 operator^=(v_f32_4x4 &a, const v_f32_4x4 &b) noexcept;
}
