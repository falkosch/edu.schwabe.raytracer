#pragma once

#include "rsqrt.h"
#include "sqr.h"
#include "staticFor.h"

#include "vectorization/constants.h"

#include "vectorization/architecture.h"

namespace vectorization {
  template <ASizeT Size, typename T>
  inline void normalize(T *const v) noexcept {
    T s = Zero<T>();

    StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
      s += sqr(v[i]);
    });

    const T rs = rsqrt(s);
    StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
      v[i] *= rs;
    });
  }
}
