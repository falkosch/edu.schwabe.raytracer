#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  // compile time static for loop for range [First, Last)
  template <ASizeT First, ASizeT Last>
  struct StaticFor {
    template <typename Lambda>
    static constexpr void apply(const Lambda &iteratee) {
      if constexpr (First < Last) {
        iteratee(First);
        if constexpr (First + 1 < Last) {
          StaticFor<First + 1, Last>::apply(iteratee);
        }
      }
    }
  };

  // Can take care of C4100 when First equals Last index
  template <ASizeT N>
  struct StaticFor<N, N> {
    template <typename Lambda>
    static constexpr void apply(const Lambda & /* iteratee */) {
    }
  };

  // compile time static for loop for range [First, Last)
  template <ASizeT First, ASizeT Last, typename Lambda>
  constexpr void staticFor(const Lambda &iteratee) noexcept {
    StaticFor<First, Last>::apply(iteratee);
  }
}
