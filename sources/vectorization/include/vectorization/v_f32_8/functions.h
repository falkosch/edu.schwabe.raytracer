#pragma once

#include "type.h"

namespace vectorization {
  v_f32_8 min(const v_f32_8 &a, const v_f32_8 &b) noexcept;
  v_f32_8 minv(const v_f32_8 &v) noexcept;
  v_f32_8::ValueType min(const v_f32_8 &v) noexcept;

  v_f32_8 max(const v_f32_8 &a, const v_f32_8 &b) noexcept;
  v_f32_8 maxv(const v_f32_8 &v) noexcept;
  v_f32_8::ValueType max(const v_f32_8 &v) noexcept;

  v_f32_8 horizontalSumv(const v_f32_8 &v) noexcept;
  v_f32_8::ValueType horizontalSum(const v_f32_8 &v) noexcept;

  v_f32_8 dotv(const v_f32_8 &a, const v_f32_8 &b) noexcept;
  v_f32_8::ValueType dot(const v_f32_8 &a, const v_f32_8 &b) noexcept;

  v_f32_8 lengthv(const v_f32_8 &v) noexcept;
  v_f32_8::ValueType length(const v_f32_8 &v) noexcept;
  v_f32_8 rlengthv(const v_f32_8 &v) noexcept;
  v_f32_8::ValueType rlength(const v_f32_8 &v) noexcept;

  v_f32_8 normalize(const v_f32_8 &v) noexcept;

  v_f32_8 distancev(const v_f32_8 &a, const v_f32_8 &b) noexcept;
  v_f32_8::ValueType distance(const v_f32_8 &a, const v_f32_8 &b) noexcept;

  v_f32_8 mix(const v_f32_8 &a, const v_f32_8 &b, const v_f32_8 &factor) noexcept;
  v_f32_8 mix(const v_f32_8 &a, const v_f32_8 &b, v_f32_8::ValueType factor) noexcept;

  v_f32_8 multiplyAdd(const v_f32_8 &a, const v_f32_8 &b, const v_f32_8 &c) noexcept;
  v_f32_8 multiplySub(const v_f32_8 &a, const v_f32_8 &b, const v_f32_8 &c) noexcept;
  v_f32_8 multiplyAddSub(const v_f32_8 &a, const v_f32_8 &b, const v_f32_8 &c) noexcept;
  v_f32_8 multiplySubAdd(const v_f32_8 &a, const v_f32_8 &b, const v_f32_8 &c) noexcept;
  v_f32_8 negativeMultiplyAdd(const v_f32_8 &a, const v_f32_8 &b, const v_f32_8 &c) noexcept;
  v_f32_8 negativeMultiplySub(const v_f32_8 &a, const v_f32_8 &b, const v_f32_8 &c) noexcept;

  v_f32_8::BoolType isNegative(const v_f32_8 &v) noexcept;
  v_f32_8::VectorBoolType isNaN(const v_f32_8 &v) noexcept;
  v_f32_8::VectorBoolType isNaN(const v_f32_8 &a, const v_f32_8 &b) noexcept;

  v_f32_8 addSubtract(const v_f32_8 &a, const v_f32_8 &b) noexcept;

  v_f32_8 sqrt(const v_f32_8 &v) noexcept;
  v_f32_8 reciprocal(const v_f32_8 &v) noexcept;
  v_f32_8 rsqrt(const v_f32_8 &v) noexcept;

  v_f32_8 ceil(const v_f32_8 &v) noexcept;
  v_f32_8 floor(const v_f32_8 &v) noexcept;
  v_f32_8 round(const v_f32_8 &v) noexcept;
  v_f32_8 fract(const v_f32_8 &v) noexcept;
  v_f32_8 modulo(const v_f32_8 &a, const v_f32_8 &b) noexcept;

  v_f32_8 abs(const v_f32_8 &v) noexcept;
  v_f32_8 clamp(const v_f32_8 &v, const v_f32_8 &lower, const v_f32_8 &upper) noexcept;

  bool anyTrue(const v_f32_8 &v) noexcept;
  bool anyFalse(const v_f32_8 &v) noexcept;
  bool allTrue(const v_f32_8 &v) noexcept;
  bool allFalse(const v_f32_8 &v) noexcept;
}
