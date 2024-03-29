#pragma once

#include "staticFor.h"

#include "vectorization/constants.h"

#include "vectorization/architecture.h"

namespace vectorization {
  template <ASizeT Size, typename T>
  inline const T average(const T *const v) noexcept {
    T s = Zero<T>();

    StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
      s += v[i];
    });

    return s / static_cast<T>(Size);
  }
}
