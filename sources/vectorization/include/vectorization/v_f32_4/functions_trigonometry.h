#pragma once

#include "type.h"

namespace vectorization {
  const v_f32_4 log(const v_f32_4 &v) noexcept;

  const v_f32_4 log3(const v_f32_4 &v) noexcept;

  const v_f32_4 exp(const v_f32_4 &v) noexcept;

  const v_f32_4 exp3(const v_f32_4 &v) noexcept;

  const v_f32_4 pow(const v_f32_4 &v, const v_f32_4 &exponent) noexcept;

  const v_f32_4 pow(const v_f32_4 &v, const v_f32_4::ValueType exponent) noexcept;

  const v_f32_4 pow3(const v_f32_4 &v, const v_f32_4 &exponent) noexcept;

  const v_f32_4 pow3(const v_f32_4 &v, const v_f32_4::ValueType exponent) noexcept;
}
