#pragma once

#include <vectorization.h>

namespace vectorization::test {
  struct Mutate {
    template <ASizeT ReplacementIndex, typename T, class InIt, class OutIt>
    static void copyAndReplaceAt(InIt first, InIt last, OutIt dest, const T &replacement) {
      std::copy(first, last, dest);
      *(dest + ReplacementIndex) = replacement;
    }
  };
}
