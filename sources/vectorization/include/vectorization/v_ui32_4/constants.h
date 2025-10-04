#pragma once

#include "type.h"

#include "vectorization/constants.h"

namespace vectorization {
  template <>
  v_ui32_4 MaskAll<v_ui32_4>() noexcept;

  template <>
  v_ui32_4 MaskX<v_ui32_4>() noexcept;

  template <>
  v_ui32_4 MaskXY<v_ui32_4>() noexcept;

  template <>
  v_ui32_4 MaskXYZ<v_ui32_4>() noexcept;

  template <>
  v_ui32_4 MaskXYZW<v_ui32_4>() noexcept;

  template <>
  v_ui32_4 MaskY<v_ui32_4>() noexcept;

  template <>
  v_ui32_4 MaskYZ<v_ui32_4>() noexcept;

  template <>
  v_ui32_4 MaskYZW<v_ui32_4>() noexcept;

  template <>
  v_ui32_4 MaskZ<v_ui32_4>() noexcept;

  template <>
  v_ui32_4 MaskZW<v_ui32_4>() noexcept;

  template <>
  v_ui32_4 MaskW<v_ui32_4>() noexcept;

  template <>
  v_ui32_4 OneW<v_ui32_4>() noexcept;

  template <>
  v_ui32_4 Zero<v_ui32_4>() noexcept;

  template <>
  v_ui32_4 One<v_ui32_4>() noexcept;

  template <>
  v_ui32_4 Two<v_ui32_4>() noexcept;
}
