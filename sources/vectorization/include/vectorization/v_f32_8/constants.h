#pragma once

#include "type.h"

#include "vectorization/constants.h"

namespace vectorization {
  template <>
  v_f32_8 DegreeToRadian<v_f32_8>() noexcept;

  template <>
  v_f32_8 Epsilon<v_f32_8>() noexcept;

  template <>
  v_f32_8 Half<v_f32_8>() noexcept;

  template <>
  v_f32_8 Infinity<v_f32_8>() noexcept;

  template <>
  v_f32_8 NegativeInfinity<v_f32_8>() noexcept;

  template <>
  v_f32_8 NegativeOne<v_f32_8>() noexcept;

  template <>
  v_f32_8 NegativeTwo<v_f32_8>() noexcept;

  template <>
  v_f32_8 NegativeZero<v_f32_8>() noexcept;

  template <>
  v_f32_8 NotANumber<v_f32_8>() noexcept;

  template <>
  v_f32_8 One<v_f32_8>() noexcept;

  template <>
  v_f32_8 OneHalf<v_f32_8>() noexcept;

  template <>
  v_f32_8 Pi<v_f32_8>() noexcept;

  template <>
  v_f32_8 RadianToDegree<v_f32_8>() noexcept;

  template <>
  v_f32_8 RadianToUniform<v_f32_8>() noexcept;

  template <>
  v_f32_8 ReciprocalPi<v_f32_8>() noexcept;

  template <>
  v_f32_8 SelfOcclusionEpsilon<v_f32_8>() noexcept;

  template <>
  v_f32_8 Sin45<v_f32_8>() noexcept;

  template <>
  v_f32_8 Two<v_f32_8>() noexcept;

  template <>
  v_f32_8 Zero<v_f32_8>() noexcept;

  template <>
  v_f32_8 MaskAll<v_f32_8>() noexcept;

  template <>
  v_f32_8 MaskNone<v_f32_8>() noexcept;
}
