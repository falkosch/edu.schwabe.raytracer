#pragma once

#include <vectorization.h>

namespace vectorization::test {
  struct Compare {
    template <typename T>
    static auto nearEqualsRelative(const T &a, const T &b) {
      const auto maxOfAbsoluteOfBoth = max(abs(a), abs(b));
      return (abs(a - b) / maxOfAbsoluteOfBoth) < Epsilon<T>();
    }

    // Up to 4 machine epsilons: the sse_mathfun / avx_mathfun polynomial approximations round differently depending on
    // the compiler's FMA contraction and reassociation (exp_ps is 1 ULP off std::exp with MSVC, 2 ULP with clang-cl)
    template <typename T>
    static auto nearEqualsRelativeApproximation(const T &a, const T &b) {
      const auto maxOfAbsoluteOfBoth = max(abs(a), abs(b));
      return (abs(a - b) / maxOfAbsoluteOfBoth) < Epsilon<T>() * Two<T>() * Two<T>();
    }

    template <typename T>
    static auto equalsOrdered(const T &a, const T &b) {
      return a == b;
    }

    template <typename T>
    static auto isBothNotANumber(const T &a, const T &b) {
      return vectorization::isNaN(a) & vectorization::isNaN(b);
    }
  };
}
