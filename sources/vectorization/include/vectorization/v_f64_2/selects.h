#pragma once

#include "type.h"

namespace vectorization {
  v_f64_2 select(bool mask, const v_f64_2 &onTrue, const v_f64_2 &onFalse) noexcept;

  v_f64_2 select(const v_f64_2::VectorBoolType &mask, const v_f64_2 &onTrue, const v_f64_2 &onFalse) noexcept;

  v_f64_2 select(const v_f64_2 &mask, const v_f64_2 &onTrue, const v_f64_2 &onFalse) noexcept;
}
