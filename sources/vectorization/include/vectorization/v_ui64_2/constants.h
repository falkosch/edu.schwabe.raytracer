#pragma once

#include "type.h"

#include "vectorization/constants.h"

namespace vectorization {
  template <>
  v_ui64_2 MaskAll<v_ui64_2>() noexcept;

  template <>
  v_ui64_2 MaskX<v_ui64_2>() noexcept;

  template <>
  v_ui64_2 MaskXY<v_ui64_2>() noexcept;

  template <>
  v_ui64_2 MaskY<v_ui64_2>() noexcept;

  template <>
  v_ui64_2 Zero<v_ui64_2>() noexcept;

  template <>
  v_ui64_2 One<v_ui64_2>() noexcept;

  template <>
  v_ui64_2 Two<v_ui64_2>() noexcept;
}
