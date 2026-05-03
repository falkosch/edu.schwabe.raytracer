#pragma once

#include "type.h"

#include "vectorization/constants.h"

namespace vectorization {
  template <>
  v_i64_2 MaskAll<v_i64_2>() noexcept;

  template <>
  v_i64_2 MaskX<v_i64_2>() noexcept;

  template <>
  v_i64_2 MaskXY<v_i64_2>() noexcept;

  template <>
  v_i64_2 MaskY<v_i64_2>() noexcept;

  template <>
  v_i64_2 Zero<v_i64_2>() noexcept;

  template <>
  v_i64_2 One<v_i64_2>() noexcept;

  template <>
  v_i64_2 NegativeOne<v_i64_2>() noexcept;
}
