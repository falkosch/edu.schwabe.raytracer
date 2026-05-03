#pragma once

#include "type.h"

namespace vectorization {
  v_i64_4 andnot(const v_i64_4 &a, const v_i64_4 &b) noexcept;

  bool anyTrue(const v_i64_4 &v) noexcept;
  bool anyFalse(const v_i64_4 &v) noexcept;
  bool allTrue(const v_i64_4 &v) noexcept;
  bool allFalse(const v_i64_4 &v) noexcept;
}
