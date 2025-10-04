#pragma once

#include "staticFor.h"

#include "vectorization/constants.h"

#include "vectorization/architecture.h"

namespace vectorization {
  template <ASizeT Size, typename T>
  T staticAverage(const T *const values) noexcept {
    auto sum = Zero<T>();

    StaticFor<VectorIndices::X, Size>::apply([&](const auto i) {
      sum += values[i];
    });

    return sum / static_cast<T>(Size);
  }
}
