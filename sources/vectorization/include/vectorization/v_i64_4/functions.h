#pragma once

#include "type.h"

namespace vectorization {
  v_i64_4 zeroW(const v_i64_4 &v) noexcept;
  v_i64_4 oneW(const v_i64_4 &v) noexcept;

  v_i64_4 andnot(const v_i64_4 &a, const v_i64_4 &b) noexcept;

  // Returns a bitmask of VectorBits indicating components with negative values, one bit per component. A set bit
  // indicates a value less than "0".
  v_i64_4::BoolType isNegative(const v_i64_4 &v) noexcept;

  v_i64_4 abs(const v_i64_4 &v) noexcept;

  v_i64_4 sqr(const v_i64_4 &v) noexcept;

  v_i64_4 min(const v_i64_4 &a, const v_i64_4 &b) noexcept;
  v_i64_4 minv(const v_i64_4 &v) noexcept;
  v_i64_4::ValueType min(const v_i64_4 &v) noexcept;
  v_i64_4 min3v(const v_i64_4 &v) noexcept;
  v_i64_4::ValueType min3(const v_i64_4 &v) noexcept;

  v_i64_4 max(const v_i64_4 &a, const v_i64_4 &b) noexcept;
  v_i64_4 maxv(const v_i64_4 &v) noexcept;
  v_i64_4::ValueType max(const v_i64_4 &v) noexcept;
  v_i64_4 max3v(const v_i64_4 &v) noexcept;
  v_i64_4::ValueType max3(const v_i64_4 &v) noexcept;

  v_i64_4 horizontalSumv(const v_i64_4 &v) noexcept;
  v_i64_4::ValueType horizontalSum(const v_i64_4 &v) noexcept;
  v_i64_4 horizontalSum3v(const v_i64_4 &v) noexcept;
  v_i64_4::ValueType horizontalSum3(const v_i64_4 &v) noexcept;

  v_i64_4 dotv(const v_i64_4 &a, const v_i64_4 &b) noexcept;
  v_i64_4::ValueType dot(const v_i64_4 &a, const v_i64_4 &b) noexcept;
  v_i64_4 dot3v(const v_i64_4 &a, const v_i64_4 &b) noexcept;
  v_i64_4::ValueType dot3(const v_i64_4 &a, const v_i64_4 &b) noexcept;

  // Integer lengths and distances are Manhattan lengths: the sum of the absolute component values.
  v_i64_4 lengthv(const v_i64_4 &v) noexcept;
  v_i64_4::ValueType length(const v_i64_4 &v) noexcept;
  v_i64_4 length3v(const v_i64_4 &v) noexcept;
  v_i64_4::ValueType length3(const v_i64_4 &v) noexcept;

  v_i64_4 distancev(const v_i64_4 &a, const v_i64_4 &b) noexcept;
  v_i64_4::ValueType distance(const v_i64_4 &a, const v_i64_4 &b) noexcept;
  v_i64_4 distance3v(const v_i64_4 &a, const v_i64_4 &b) noexcept;
  v_i64_4::ValueType distance3(const v_i64_4 &a, const v_i64_4 &b) noexcept;

  // The multiply-add family wraps around on overflow like the integer operators.
  // a * b + c
  v_i64_4 multiplyAdd(const v_i64_4 &a, const v_i64_4 &b, const v_i64_4 &c) noexcept;
  // a * b - c
  v_i64_4 multiplySub(const v_i64_4 &a, const v_i64_4 &b, const v_i64_4 &c) noexcept;
  // a * b with alternating sub/add of c: {a.x*b.x-c.x, a.y*b.y+c.y, a.z*b.z-c.z, a.w*b.w+c.w}
  v_i64_4 multiplyAddSub(const v_i64_4 &a, const v_i64_4 &b, const v_i64_4 &c) noexcept;
  // a * b with alternating add/sub of c: {a.x*b.x+c.x, a.y*b.y-c.y, a.z*b.z+c.z, a.w*b.w-c.w}
  v_i64_4 multiplySubAdd(const v_i64_4 &a, const v_i64_4 &b, const v_i64_4 &c) noexcept;
  // -(a * b) + c
  v_i64_4 negativeMultiplyAdd(const v_i64_4 &a, const v_i64_4 &b, const v_i64_4 &c) noexcept;
  // -(a * b) - c
  v_i64_4 negativeMultiplySub(const v_i64_4 &a, const v_i64_4 &b, const v_i64_4 &c) noexcept;

  // Alternately subtracts and adds: {a.x - b.x, a.y + b.y, a.z - b.z, a.w + b.w}
  v_i64_4 addSubtract(const v_i64_4 &a, const v_i64_4 &b) noexcept;

  // Remainder of the truncating division per component, like operator%: the result has the sign of a.
  v_i64_4 modulo(const v_i64_4 &a, const v_i64_4 &b) noexcept;

  v_i64_4 clamp(const v_i64_4 &v, const v_i64_4 &lower, const v_i64_4 &upper) noexcept;

  // Index of the smallest/largest component; of several equal ones, the highest index.
  ASizeT argmin(const v_i64_4 &v) noexcept;
  ASizeT argmax(const v_i64_4 &v) noexcept;
  ASizeT argmin3(const v_i64_4 &v) noexcept;
  ASizeT argmax3(const v_i64_4 &v) noexcept;

  // The tests are bitwise: allTrue needs all bits of the tested components set, anyTrue any bit, allFalse none and
  // anyFalse not all. Comparison masks set all or none of a component's bits.
  bool anyTrue(const v_i64_4 &v) noexcept;
  bool anyTrue3(const v_i64_4 &v) noexcept;
  bool anyFalse(const v_i64_4 &v) noexcept;
  bool anyFalse3(const v_i64_4 &v) noexcept;
  bool allTrue(const v_i64_4 &v) noexcept;
  bool allTrue3(const v_i64_4 &v) noexcept;
  bool allFalse(const v_i64_4 &v) noexcept;
  bool allFalse3(const v_i64_4 &v) noexcept;
}
