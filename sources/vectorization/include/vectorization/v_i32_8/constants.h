#pragma once

#include "type.h"

#include "vectorization/constants/values/negative_one.h"
#include "vectorization/constants/values/negative_two.h"
#include "vectorization/constants/values/one.h"
#include "vectorization/constants/values/two.h"
#include "vectorization/constants/values/zero.h"

namespace vectorization {
  template <>
  v_i32_8 Zero<v_i32_8>() noexcept;

  template <>
  v_i32_8 One<v_i32_8>() noexcept;

  template <>
  v_i32_8 Two<v_i32_8>() noexcept;

  template <>
  v_i32_8 NegativeOne<v_i32_8>() noexcept;

  template <>
  v_i32_8 NegativeTwo<v_i32_8>() noexcept;
}
