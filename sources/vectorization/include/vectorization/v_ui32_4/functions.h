#pragma once

#include "type.h"

namespace vectorization {
  v_ui32_4 zeroW(const v_ui32_4 &vector) noexcept;

  v_ui32_4 lengthv(const v_ui32_4 &vector) noexcept;
  v_ui32_4 length3v(const v_ui32_4 &vector) noexcept;
  v_ui32_4::ValueType length(const v_ui32_4 &vector) noexcept;
  v_ui32_4::ValueType length3(const v_ui32_4 &vector) noexcept;

  v_ui32_4::ValueType distance(const v_ui32_4 &a, const v_ui32_4 &b) noexcept;
  v_ui32_4::ValueType distance3(const v_ui32_4 &a, const v_ui32_4 &b) noexcept;

  v_ui32_4 min(const v_ui32_4 &a, const v_ui32_4 &b) noexcept;
  v_ui32_4::ValueType min(const v_ui32_4 &vector) noexcept;
  v_ui32_4::ValueType min3(const v_ui32_4 &vector) noexcept;

  v_ui32_4 max(const v_ui32_4 &a, const v_ui32_4 &b) noexcept;
  v_ui32_4::ValueType max(const v_ui32_4 &vector) noexcept;
  v_ui32_4::ValueType max3(const v_ui32_4 &vector) noexcept;

  v_ui32_4 clamp(const v_ui32_4 &vector, const v_ui32_4 &lowerVector, const v_ui32_4 &upperVector) noexcept;

  bool anyTrue(const v_ui32_4 &vector) noexcept;
  bool anyTrue3(const v_ui32_4 &vector) noexcept;
  bool anyFalse(const v_ui32_4 &vector) noexcept;
  bool anyFalse3(const v_ui32_4 &vector) noexcept;

  bool allTrue(const v_ui32_4 &vector) noexcept;
  bool allTrue3(const v_ui32_4 &vector) noexcept;
  bool allFalse(const v_ui32_4 &vector) noexcept;
  bool allFalse3(const v_ui32_4 &vector) noexcept;
}
