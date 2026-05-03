#pragma once

#include "type.h"

#include "vectorization/constants/values/negative_one.h"
#include "vectorization/constants/values/one.h"
#include "vectorization/constants/values/zero.h"

namespace vectorization {
  template <>
  v_i64_4 Zero<v_i64_4>() noexcept;

  template <>
  v_i64_4 One<v_i64_4>() noexcept;

  template <>
  v_i64_4 NegativeOne<v_i64_4>() noexcept;
}
