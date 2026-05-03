#pragma once

#include "type.h"

namespace vectorization {
  v_f32_8 log(const v_f32_8 &v) noexcept;

  v_f32_8 exp(const v_f32_8 &v) noexcept;

  v_f32_8 pow(const v_f32_8 &v, const v_f32_8 &exponent) noexcept;
  v_f32_8 pow(const v_f32_8 &v, v_f32_8::ValueType exponent) noexcept;

  v_f32_8 sin(const v_f32_8 &v) noexcept;

  v_f32_8 cos(const v_f32_8 &v) noexcept;

  void sincos(const v_f32_8 &v, v_f32_8 &outSin, v_f32_8 &outCos) noexcept;
}
