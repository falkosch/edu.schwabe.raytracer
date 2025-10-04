#pragma once

#include "type.h"

namespace vectorization {
  v_ui64_2 min(const v_ui64_2 &a, const v_ui64_2 &b) noexcept;
  v_ui64_2 minv(const v_ui64_2 &vector) noexcept;
  v_ui64_2::ValueType min(const v_ui64_2 &vector) noexcept;

  v_ui64_2 max(const v_ui64_2 &a, const v_ui64_2 &b) noexcept;
  v_ui64_2 maxv(const v_ui64_2 &vector) noexcept;
  v_ui64_2::ValueType max(const v_ui64_2 &vector) noexcept;

  v_ui64_2 lengthv(const v_ui64_2 &vector) noexcept;
  v_ui64_2::ValueType length(const v_ui64_2 &vector) noexcept;

  v_ui64_2::ValueType distance(const v_ui64_2 &a, const v_ui64_2 &b) noexcept;

  v_ui64_2 clamp(const v_ui64_2 &vector, const v_ui64_2 &lowerVector, const v_ui64_2 &upperVector) noexcept;

  bool anyTrue(const v_ui64_2 &vector) noexcept;
  bool anyFalse(const v_ui64_2 &vector) noexcept;
  bool allTrue(const v_ui64_2 &vector) noexcept;
  bool allFalse(const v_ui64_2 &vector) noexcept;
}
