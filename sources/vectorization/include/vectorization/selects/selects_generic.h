#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <typename T>
  constexpr T boolMask(const bool mask) noexcept {
    static_assert(std::is_integral_v<T>, "T must be an integral type");
    return static_cast<T>(-static_cast<std::make_signed_t<T>>(mask));
  }

  template <typename T>
  constexpr T select(const bool mask, const T &onTrue, const T &onFalse) noexcept {
    return *(mask ? &onTrue : &onFalse);
  }
}
