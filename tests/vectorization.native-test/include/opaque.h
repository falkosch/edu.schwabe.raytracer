#pragma once

#include <cstddef>
#include <type_traits>

namespace vectorization::test {
  // Returns value unchanged but hides it from the optimizer. Under /fp:fast (and clang's fast-math flags) the compiler may
  // fold arithmetic on compile-time constants with non-IEEE rules (-0 + 0 -> -0, 0 * inf -> 0, x - x -> 0). Tests that
  // check the IEEE behaviour of the SIMD instructions for special values (-0, inf, NaN) pass their operands through
  // opaque() so the operation is executed at runtime.
  template <typename T>
  T opaque(const T &value) noexcept {
    static_assert(std::is_trivially_copyable_v<T>);
    volatile unsigned char bytes[sizeof(T)];
    const auto *source = reinterpret_cast<const unsigned char *>(&value);
    for (std::size_t i = 0; i < sizeof(T); ++i) {
      bytes[i] = source[i];
    }
    T result{};
    auto *target = reinterpret_cast<unsigned char *>(&result);
    for (std::size_t i = 0; i < sizeof(T); ++i) {
      target[i] = bytes[i];
    }
    return result;
  }
}
