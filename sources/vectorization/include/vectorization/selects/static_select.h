#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <typename T>
  inline constexpr T boolMask(const bool mask) noexcept {
    static_assert(std::is_integral<T>::value, "T must be an integral type");
    return static_cast<T>(-static_cast<typename std::make_signed<T>::type>(mask));
  }

  template <typename T>
  inline constexpr T select(const bool mask, const T &onTrue, const T &onFalse) noexcept {
    return *(mask ? &onTrue : &onFalse);
  }
}
