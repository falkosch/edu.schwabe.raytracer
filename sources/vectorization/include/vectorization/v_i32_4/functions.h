#pragma once

#include "type.h"

namespace vectorization {
  v_i32_4 zeroW(const v_i32_4 &v) noexcept;

  // returns ((~a) & b)
  v_i32_4 andnot(const v_i32_4 &a, const v_i32_4 &b) noexcept;

  // Returns a bitmask indicating components with negative values. A set bit indicates a value
  // less than "0".
  v_i32_4::BoolType isNegative(const v_i32_4 &v) noexcept;

  v_i32_4 abs(const v_i32_4 &v) noexcept;

  v_i32_4 lengthv(const v_i32_4 &v) noexcept;
  v_i32_4 length3v(const v_i32_4 &v) noexcept;
  v_i32_4::ValueType length(const v_i32_4 &v) noexcept;
  v_i32_4::ValueType length3(const v_i32_4 &v) noexcept;

  v_i32_4::ValueType distance(const v_i32_4 &a, const v_i32_4 &b) noexcept;
  v_i32_4::ValueType distance3(const v_i32_4 &a, const v_i32_4 &b) noexcept;

  v_i32_4 min(const v_i32_4 &a, const v_i32_4 &b) noexcept;
  v_i32_4::ValueType min(const v_i32_4 &v) noexcept;
  v_i32_4::ValueType min3(const v_i32_4 &v) noexcept;

  v_i32_4 max(const v_i32_4 &a, const v_i32_4 &b) noexcept;
  v_i32_4::ValueType max(const v_i32_4 &v) noexcept;
  v_i32_4::ValueType max3(const v_i32_4 &v) noexcept;

  v_i32_4 clamp(const v_i32_4 &v, const v_i32_4 &l, const v_i32_4 &m) noexcept;

  bool anyTrue(const v_i32_4 &v) noexcept;
  bool anyTrue3(const v_i32_4 &v) noexcept;
  bool anyFalse(const v_i32_4 &v) noexcept;
  bool anyFalse3(const v_i32_4 &v) noexcept;

  bool allTrue(const v_i32_4 &v) noexcept;
  bool allTrue3(const v_i32_4 &v) noexcept;
  bool allFalse(const v_i32_4 &v) noexcept;
  bool allFalse3(const v_i32_4 &v) noexcept;
}
