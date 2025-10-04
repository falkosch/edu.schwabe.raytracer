#pragma once

#include "api.h"

#include "vectorization/constants.h"

namespace vectorization {
  template <>
  v_f32_4x4 Zero<v_f32_4x4>() noexcept;
}
