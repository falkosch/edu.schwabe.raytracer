#pragma once

#include "type.h"

#include "vectorization/constants/values/one.h"
#include "vectorization/constants/values/two.h"
#include "vectorization/constants/values/zero.h"

namespace vectorization {
  template <>
  v_ui32_8 Zero<v_ui32_8>() noexcept;

  template <>
  v_ui32_8 One<v_ui32_8>() noexcept;

  template <>
  v_ui32_8 Two<v_ui32_8>() noexcept;
}
