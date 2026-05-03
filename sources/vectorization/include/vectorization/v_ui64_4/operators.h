#pragma once

#include "type.h"

#include <ostream>

namespace vectorization {

  v_ui64_4 operator!(const v_ui64_4 &v) noexcept;

  v_ui64_4 operator~(const v_ui64_4 &v) noexcept;

  v_ui64_4 operator+(const v_ui64_4 &a, const v_ui64_4 &b) noexcept;

  v_ui64_4 operator-(const v_ui64_4 &a, const v_ui64_4 &b) noexcept;

  v_ui64_4 operator&(const v_ui64_4 &a, const v_ui64_4 &b) noexcept;

  v_ui64_4 operator|(const v_ui64_4 &a, const v_ui64_4 &b) noexcept;

  v_ui64_4 operator^(const v_ui64_4 &a, const v_ui64_4 &b) noexcept;

  v_ui64_4 operator>>(const v_ui64_4 &a, const v_ui64_4 &b) noexcept;

  v_ui64_4 operator<(const v_ui64_4 &a, const v_ui64_4 &b) noexcept;

  v_ui64_4 operator>(const v_ui64_4 &a, const v_ui64_4 &b) noexcept;

  v_ui64_4 operator==(const v_ui64_4 &a, const v_ui64_4 &b) noexcept;

  v_ui64_4 operator!=(const v_ui64_4 &a, const v_ui64_4 &b) noexcept;

  v_ui64_4 &operator+=(v_ui64_4 &a, const v_ui64_4 &b) noexcept;

  v_ui64_4 &operator-=(v_ui64_4 &a, const v_ui64_4 &b) noexcept;

  v_ui64_4 &operator&=(v_ui64_4 &a, const v_ui64_4 &b) noexcept;

  v_ui64_4 &operator|=(v_ui64_4 &a, const v_ui64_4 &b) noexcept;

  v_ui64_4 &operator^=(v_ui64_4 &a, const v_ui64_4 &b) noexcept;

  v_ui64_4 &operator>>=(v_ui64_4 &a, const v_ui64_4 &b) noexcept;

  std::ostream &operator<<(std::ostream &stream, const v_ui64_4 &v);
}
