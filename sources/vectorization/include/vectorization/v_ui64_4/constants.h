#pragma once

#include "type.h"

#include "vectorization/constants/values/one.h"
#include "vectorization/constants/values/zero.h"

namespace vectorization {
  template <>
  v_ui64_4 Zero<v_ui64_4>() noexcept;

  template <>
  v_ui64_4 One<v_ui64_4>() noexcept;
}
