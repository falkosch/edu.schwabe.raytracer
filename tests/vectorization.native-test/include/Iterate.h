#pragma once

#include "MessageFormat.h"

#include <vectorization.h>

#include <functional>
#include <ostream>
#include <string>

namespace vectorization::test {
  struct Iterate {
    template <ASizeT exclusiveEnd>
    static void toExclusiveEnd(
        std::function<void(const ASizeT index, std::wstring)> test,
        std::function<std::ostream &(std::ostream &)> writeMessage
    ) {
      staticFor<VectorIndices::X, exclusiveEnd>([&](auto i) {
        test(i, MessageFormat::forIndex(i, writeMessage));
      });
    }

    template <ASizeT exclusiveEnd>
    static void toExclusiveEnd(std::function<void(const ASizeT index, std::wstring)> test) {
      Iterate::toExclusiveEnd<exclusiveEnd>(test, [&](auto &stream) -> auto & {
        return stream;
      });
    }

    template <ASizeT exclusiveEnd>
    static void toExclusiveEnd(std::function<void(const ASizeT index, std::wstring)> test, std::string message) {
      Iterate::toExclusiveEnd<exclusiveEnd>(test, [&](auto &stream) -> auto & {
        return stream << " --- " << message;
      });
    }
  };
}
