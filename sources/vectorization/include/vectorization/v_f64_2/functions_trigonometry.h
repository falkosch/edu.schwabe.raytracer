#pragma once

#include "type.h"

namespace vectorization {
  v_f64_2 log(const v_f64_2 &v) noexcept;

  v_f64_2 exp(const v_f64_2 &v) noexcept;

  v_f64_2 pow(const v_f64_2 &v, const v_f64_2 &exponent) noexcept;
  v_f64_2 pow(const v_f64_2 &v, v_f64_2::ValueType exponent) noexcept;

  v_f64_2 sin(const v_f64_2 &v) noexcept;

  v_f64_2 cos(const v_f64_2 &v) noexcept;

  void sincos(const v_f64_2 &v, v_f64_2 &outSin, v_f64_2 &outCos) noexcept;
}
