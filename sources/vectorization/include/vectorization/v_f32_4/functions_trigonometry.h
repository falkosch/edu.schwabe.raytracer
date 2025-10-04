#pragma once

#include "type.h"

namespace vectorization {
  v_f32_4 log(const v_f32_4 &v) noexcept;
  v_f32_4 log3(const v_f32_4 &v) noexcept;

  v_f32_4 exp(const v_f32_4 &v) noexcept;
  v_f32_4 exp3(const v_f32_4 &v) noexcept;

  v_f32_4 pow(const v_f32_4 &v, const v_f32_4 &exponent) noexcept;
  v_f32_4 pow(const v_f32_4 &v, v_f32_4::ValueType exponent) noexcept;
  v_f32_4 pow3(const v_f32_4 &v, const v_f32_4 &exponent) noexcept;
  v_f32_4 pow3(const v_f32_4 &v, v_f32_4::ValueType exponent) noexcept;
}
