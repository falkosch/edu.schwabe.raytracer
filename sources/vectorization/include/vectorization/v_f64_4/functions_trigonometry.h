#pragma once

#include "type.h"

namespace vectorization {
  v_f64_4 log(const v_f64_4 &v) noexcept;

  v_f64_4 exp(const v_f64_4 &v) noexcept;

  v_f64_4 pow(const v_f64_4 &v, const v_f64_4 &exponent) noexcept;
  v_f64_4 pow(const v_f64_4 &v, v_f64_4::ValueType exponent) noexcept;

  v_f64_4 sin(const v_f64_4 &v) noexcept;

  v_f64_4 cos(const v_f64_4 &v) noexcept;

  void sincos(const v_f64_4 &v, v_f64_4 &outSin, v_f64_4 &outCos) noexcept;
}
