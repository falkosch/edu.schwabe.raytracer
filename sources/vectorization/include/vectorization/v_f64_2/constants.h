#pragma once

#include "type.h"

#include "vectorization/constants.h"

namespace vectorization {
  template <>
  v_f64_2 MaskAll<v_f64_2>() noexcept;

  template <>
  v_f64_2 MaskNone<v_f64_2>() noexcept;

  template <>
  v_f64_2 MaskX<v_f64_2>() noexcept;

  template <>
  v_f64_2 MaskXY<v_f64_2>() noexcept;

  template <>
  v_f64_2 MaskY<v_f64_2>() noexcept;

  template <>
  v_f64_2 DegreeToRadian<v_f64_2>() noexcept;

  template <>
  v_f64_2 Epsilon<v_f64_2>() noexcept;

  template <>
  v_f64_2 Half<v_f64_2>() noexcept;

  template <>
  v_f64_2 Infinity<v_f64_2>() noexcept;

  template <>
  v_f64_2 NegativeInfinity<v_f64_2>() noexcept;

  template <>
  v_f64_2 NegativeOne<v_f64_2>() noexcept;

  template <>
  v_f64_2 NegativeTwo<v_f64_2>() noexcept;

  template <>
  v_f64_2 NegativeZero<v_f64_2>() noexcept;

  template <>
  v_f64_2 NotANumber<v_f64_2>() noexcept;

  template <>
  v_f64_2 One<v_f64_2>() noexcept;

  template <>
  v_f64_2 OneHalf<v_f64_2>() noexcept;

  template <>
  v_f64_2 OneX<v_f64_2>() noexcept;

  template <>
  v_f64_2 OneY<v_f64_2>() noexcept;

  template <>
  v_f64_2 Pi<v_f64_2>() noexcept;

  template <>
  v_f64_2 RadianToDegree<v_f64_2>() noexcept;

  template <>
  v_f64_2 RadianToUniform<v_f64_2>() noexcept;

  template <>
  v_f64_2 ReciprocalPi<v_f64_2>() noexcept;

  template <>
  v_f64_2 SelfOcclusionEpsilon<v_f64_2>() noexcept;

  template <>
  v_f64_2 Sin45<v_f64_2>() noexcept;

  template <>
  v_f64_2 Two<v_f64_2>() noexcept;

  template <>
  v_f64_2 Zero<v_f64_2>() noexcept;
}
