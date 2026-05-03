#pragma once

#include "type.h"

namespace vectorization {
  v_f32_8 select(bool mask, const v_f32_8 &onTrue, const v_f32_8 &onFalse) noexcept;

  v_f32_8 select(const v_f32_8::VectorBoolType &mask, const v_f32_8 &onTrue, const v_f32_8 &onFalse) noexcept;

  v_f32_8 select(const v_f32_8 &mask, const v_f32_8 &onTrue, const v_f32_8 &onFalse) noexcept;
}
