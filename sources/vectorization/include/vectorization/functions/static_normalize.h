#pragma once

#include "rsqrt.h"
#include "sqr.h"
#include "static_for.h"

#include "vectorization/constants.h"

#include "vectorization/architecture.h"

namespace vectorization {
  template <ASizeT Size, typename T>
  void staticNormalize(T *const values) noexcept {
    auto squaredSum = Zero<T>();

    StaticFor<VectorIndices::X, Size>::apply([&](const auto i) {
      squaredSum += sqr(values[i]);
    });

    const auto rs = rsqrt(squaredSum);
    StaticFor<VectorIndices::X, Size>::apply([&](const auto i) {
      values[i] *= rs;
    });
  }
}
