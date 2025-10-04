#pragma once

#include "type.h"

namespace vectorization {
  template <>
  v_i32_4 MaskAll<v_i32_4>() noexcept;

  template <>
  v_i32_4 MaskX<v_i32_4>() noexcept;

  template <>
  v_i32_4 MaskXY<v_i32_4>() noexcept;

  template <>
  v_i32_4 MaskXYZ<v_i32_4>() noexcept;

  template <>
  v_i32_4 MaskXYZW<v_i32_4>() noexcept;

  template <>
  v_i32_4 MaskY<v_i32_4>() noexcept;

  template <>
  v_i32_4 MaskYZ<v_i32_4>() noexcept;

  template <>
  v_i32_4 MaskYZW<v_i32_4>() noexcept;

  template <>
  v_i32_4 MaskZ<v_i32_4>() noexcept;

  template <>
  v_i32_4 MaskZW<v_i32_4>() noexcept;

  template <>
  v_i32_4 MaskW<v_i32_4>() noexcept;

  template <>
  v_i32_4 OneW<v_i32_4>() noexcept;

  template <>
  v_i32_4 Zero<v_i32_4>() noexcept;

  template <>
  v_i32_4 One<v_i32_4>() noexcept;

  template <>
  v_i32_4 Two<v_i32_4>() noexcept;

  template <>
  v_i32_4 NegativeOne<v_i32_4>() noexcept;

  template <>
  v_i32_4 NegativeTwo<v_i32_4>() noexcept;
}
