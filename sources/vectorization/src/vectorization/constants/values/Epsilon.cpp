#include "vectorization/constants/values/Epsilon.h"

#include "vectorization/constants/values/Half.h"
#include "vectorization/constants/values/One.h"

#include <iostream>

namespace vectorization {
  template <typename T>
  T benchmarkMachineEpsilon() noexcept {
    T f = One<T>();
    T e = f;
    while (f + One<T>() != One<T>()) {
      e = f;
      f *= Half<T>();
    }
    std::cout << "Machine epsilon for " << typeid(T).name() << " is " << e << std::endl;
    return e;
  }

  const Float_32 Epsilon_Float_32 = benchmarkMachineEpsilon<Float_32>();
  const Float_64 Epsilon_Float_64 = benchmarkMachineEpsilon<Float_64>();

  template <>
  Float_32 Epsilon<Float_32>() noexcept {
    return Epsilon_Float_32;
  }

  template <>
  Float_64 Epsilon<Float_64>() noexcept {
    return Epsilon_Float_64;
  }

  template <>
  PackedFloat2_128 Epsilon<PackedFloat2_128>() noexcept {
    return _mm_set1_pd(Epsilon_Float_64);
  }

  template <>
  PackedFloat4_128 Epsilon<PackedFloat4_128>() noexcept {
    return _mm_set1_ps(Epsilon_Float_32);
  }

  template <>
  PackedFloat4_256 Epsilon<PackedFloat4_256>() noexcept {
    return _mm256_set1_pd(Epsilon_Float_64);
  }

  template <>
  PackedFloat8_256 Epsilon<PackedFloat8_256>() noexcept {
    return _mm256_set1_ps(Epsilon_Float_32);
  }
}
