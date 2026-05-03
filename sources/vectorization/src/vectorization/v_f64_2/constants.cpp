#include "vectorization/v_f64_2/constants.h"

namespace vectorization {
  template <>
  v_f64_2 MaskAll<v_f64_2>() noexcept {
    return MaskAll<v_f64_2::PackedType>();
  }

  template <>
  v_f64_2 MaskNone<v_f64_2>() noexcept {
    return MaskNone<v_f64_2::PackedType>();
  }

  template <>
  v_f64_2 MaskX<v_f64_2>() noexcept {
    return MaskX<v_f64_2::PackedType>();
  }

  template <>
  v_f64_2 MaskXY<v_f64_2>() noexcept {
    return MaskXY<v_f64_2::PackedType>();
  }

  template <>
  v_f64_2 MaskY<v_f64_2>() noexcept {
    return MaskY<v_f64_2::PackedType>();
  }

  template <>
  v_f64_2 DegreeToRadian<v_f64_2>() noexcept {
    return DegreeToRadian<v_f64_2::PackedType>();
  }

  template <>
  v_f64_2 Epsilon<v_f64_2>() noexcept {
    return Epsilon<v_f64_2::PackedType>();
  }

  template <>
  v_f64_2 Half<v_f64_2>() noexcept {
    return Half<v_f64_2::PackedType>();
  }

  template <>
  v_f64_2 Infinity<v_f64_2>() noexcept {
    return Infinity<v_f64_2::PackedType>();
  }

  template <>
  v_f64_2 NegativeInfinity<v_f64_2>() noexcept {
    return NegativeInfinity<v_f64_2::PackedType>();
  }

  template <>
  v_f64_2 NegativeOne<v_f64_2>() noexcept {
    return NegativeOne<v_f64_2::PackedType>();
  }

  template <>
  v_f64_2 NegativeTwo<v_f64_2>() noexcept {
    return NegativeTwo<v_f64_2::PackedType>();
  }

  template <>
  v_f64_2 NegativeZero<v_f64_2>() noexcept {
    return NegativeZero<v_f64_2::PackedType>();
  }

  template <>
  v_f64_2 NotANumber<v_f64_2>() noexcept {
    return NotANumber<v_f64_2::PackedType>();
  }

  template <>
  v_f64_2 One<v_f64_2>() noexcept {
    return One<v_f64_2::PackedType>();
  }

  template <>
  v_f64_2 OneHalf<v_f64_2>() noexcept {
    return OneHalf<v_f64_2::PackedType>();
  }

  template <>
  v_f64_2 OneX<v_f64_2>() noexcept {
    return OneX<v_f64_2::PackedType>();
  }

  template <>
  v_f64_2 OneY<v_f64_2>() noexcept {
    return OneY<v_f64_2::PackedType>();
  }

  template <>
  v_f64_2 Pi<v_f64_2>() noexcept {
    return Pi<v_f64_2::PackedType>();
  }

  template <>
  v_f64_2 RadianToDegree<v_f64_2>() noexcept {
    return RadianToDegree<v_f64_2::PackedType>();
  }

  template <>
  v_f64_2 RadianToUniform<v_f64_2>() noexcept {
    return RadianToUniform<v_f64_2::PackedType>();
  }

  template <>
  v_f64_2 ReciprocalPi<v_f64_2>() noexcept {
    return ReciprocalPi<v_f64_2::PackedType>();
  }

  template <>
  v_f64_2 SelfOcclusionEpsilon<v_f64_2>() noexcept {
    return SelfOcclusionEpsilon<v_f64_2::PackedType>();
  }

  template <>
  v_f64_2 Sin45<v_f64_2>() noexcept {
    return Sin45<v_f64_2::PackedType>();
  }

  template <>
  v_f64_2 Two<v_f64_2>() noexcept {
    return Two<v_f64_2::PackedType>();
  }

  template <>
  v_f64_2 Zero<v_f64_2>() noexcept {
    return Zero<v_f64_2::PackedType>();
  }
}
