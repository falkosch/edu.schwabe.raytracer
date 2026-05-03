#pragma once

#include "type.h"

namespace vectorization {
  v_f64_4 select(bool mask, const v_f64_4 &onTrue, const v_f64_4 &onFalse) noexcept;

  v_f64_4 select(const v_f64_4::VectorBoolType &mask, const v_f64_4 &onTrue, const v_f64_4 &onFalse) noexcept;

  v_f64_4 select(const v_f64_4 &mask, const v_f64_4 &onTrue, const v_f64_4 &onFalse) noexcept;
}
