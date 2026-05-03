#pragma once

#include "type.h"

namespace vectorization {
  v_ui64_4 andnot(const v_ui64_4 &a, const v_ui64_4 &b) noexcept;

  v_ui64_4 min(const v_ui64_4 &a, const v_ui64_4 &b) noexcept;
  v_ui64_4 max(const v_ui64_4 &a, const v_ui64_4 &b) noexcept;

  bool anyTrue(const v_ui64_4 &v) noexcept;
  bool anyFalse(const v_ui64_4 &v) noexcept;
  bool allTrue(const v_ui64_4 &v) noexcept;
  bool allFalse(const v_ui64_4 &v) noexcept;
}
