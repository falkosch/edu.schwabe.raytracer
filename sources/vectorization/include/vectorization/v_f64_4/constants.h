#pragma once

#include "type.h"

#include "vectorization/constants.h"

namespace vectorization {
  template <>
  v_f64_4 DegreeToRadian<v_f64_4>() noexcept;

  template <>
  v_f64_4 Epsilon<v_f64_4>() noexcept;

  template <>
  v_f64_4 Half<v_f64_4>() noexcept;

  template <>
  v_f64_4 Infinity<v_f64_4>() noexcept;

  template <>
  v_f64_4 NegativeInfinity<v_f64_4>() noexcept;

  template <>
  v_f64_4 NegativeOne<v_f64_4>() noexcept;

  template <>
  v_f64_4 NegativeTwo<v_f64_4>() noexcept;

  template <>
  v_f64_4 NegativeZero<v_f64_4>() noexcept;

  template <>
  v_f64_4 NotANumber<v_f64_4>() noexcept;

  template <>
  v_f64_4 One<v_f64_4>() noexcept;

  template <>
  v_f64_4 OneHalf<v_f64_4>() noexcept;

  template <>
  v_f64_4 Pi<v_f64_4>() noexcept;

  template <>
  v_f64_4 RadianToDegree<v_f64_4>() noexcept;

  template <>
  v_f64_4 RadianToUniform<v_f64_4>() noexcept;

  template <>
  v_f64_4 ReciprocalPi<v_f64_4>() noexcept;

  template <>
  v_f64_4 SelfOcclusionEpsilon<v_f64_4>() noexcept;

  template <>
  v_f64_4 Sin45<v_f64_4>() noexcept;

  template <>
  v_f64_4 Two<v_f64_4>() noexcept;

  template <>
  v_f64_4 Zero<v_f64_4>() noexcept;

  template <>
  v_f64_4 MaskAll<v_f64_4>() noexcept;

  template <>
  v_f64_4 MaskNone<v_f64_4>() noexcept;
}
