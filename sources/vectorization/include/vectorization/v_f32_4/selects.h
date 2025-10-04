#pragma once

#include "type.h"

namespace vectorization {
  const v_f32_4 select(const bool mask, const v_f32_4 &onTrue, const v_f32_4 &onFalse) noexcept;

  const v_f32_4 select(const v_f32_4::VectorBoolType &mask, const v_f32_4 &onTrue, const v_f32_4 &onFalse) noexcept;

  const v_f32_4 select(const v_f32_4 &mask, const v_f32_4 &onTrue, const v_f32_4 &onFalse) noexcept;
}
