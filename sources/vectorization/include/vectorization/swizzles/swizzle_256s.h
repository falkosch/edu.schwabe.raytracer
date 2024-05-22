#pragma once

#include "vectorization/architecture.h"
#include "vectorization/blends.h"

namespace vectorization {
  template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W>
  // generic swizzle for both PackedFloat4_128
  inline PackedFloat8_256 swizzle(const PackedFloat8_256 &values) noexcept {
    static_assert(X < VectorSizes::W, "Index X is out of range");
    static_assert(Y < VectorSizes::W, "Index Y is out of range");
    static_assert(Z < VectorSizes::W, "Index Z is out of range");
    static_assert(W < VectorSizes::W, "Index W is out of range");

    constexpr auto PermuteMask = X | (Y << 2) | (Z << 4) | (W << 6);
    return _mm256_permute_ps(values, PermuteMask);
  }

  template <>
  // passthrough
  PackedFloat8_256
  swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(const PackedFloat8_256 &values
  ) noexcept;

  template <>
  // duplicate X1(v), X1(v), X2(v), X2(v), X5(v), X5(v), X6(v), X6(v)
  PackedFloat8_256
  swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y>(const PackedFloat8_256 &values
  ) noexcept;

  template <>
  // duplicate X1(v), X1(v), X3(v), X3(v), X5(v), X5(v), X7(v), X7(v)
  PackedFloat8_256
  swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Z, VectorIndices::Z>(const PackedFloat8_256 &values
  ) noexcept;

  template <>
  // duplicate X2(v), X2(v), X4(v), X4(v), X6(v), X6(v), X8(v), X8(v)
  PackedFloat8_256
  swizzle<VectorIndices::Y, VectorIndices::Y, VectorIndices::W, VectorIndices::W>(const PackedFloat8_256 &values
  ) noexcept;

  template <>
  // duplicate X3(v), X3(v), X4(v), X4(v), X7(v), X7(v), X8(v), X8(v)
  PackedFloat8_256
  swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(const PackedFloat8_256 &values
  ) noexcept;

  PackedFloat8_256 xxxx(const PackedFloat8_256 &values) noexcept;
  PackedFloat8_256 xxxz(const PackedFloat8_256 &values) noexcept;
  PackedFloat8_256 xxyy(const PackedFloat8_256 &values) noexcept;
  PackedFloat8_256 xxzz(const PackedFloat8_256 &values) noexcept;
  PackedFloat8_256 xyxy(const PackedFloat8_256 &values) noexcept;
  PackedFloat8_256 xzzz(const PackedFloat8_256 &values) noexcept;

  PackedFloat8_256 yyyy(const PackedFloat8_256 &values) noexcept;
  PackedFloat8_256 yxxy(const PackedFloat8_256 &values) noexcept;
  PackedFloat8_256 yxwz(const PackedFloat8_256 &values) noexcept;
  PackedFloat8_256 yyww(const PackedFloat8_256 &values) noexcept;
  PackedFloat8_256 yzxw(const PackedFloat8_256 &values) noexcept;
  PackedFloat8_256 yzwx(const PackedFloat8_256 &values) noexcept;

  PackedFloat8_256 zzzz(const PackedFloat8_256 &values) noexcept;
  PackedFloat8_256 zxyw(const PackedFloat8_256 &values) noexcept;
  PackedFloat8_256 zzyy(const PackedFloat8_256 &values) noexcept;
  PackedFloat8_256 zzww(const PackedFloat8_256 &values) noexcept;
  PackedFloat8_256 zwxy(const PackedFloat8_256 &values) noexcept;
  PackedFloat8_256 zwzw(const PackedFloat8_256 &values) noexcept;

  PackedFloat8_256 wwww(const PackedFloat8_256 &values) noexcept;
  PackedFloat8_256 wxyz(const PackedFloat8_256 &values) noexcept;
  PackedFloat8_256 wzyx(const PackedFloat8_256 &values) noexcept;

  template <ASizeT X1, ASizeT X2, ASizeT X3, ASizeT X4, ASizeT X5, ASizeT X6, ASizeT X7, ASizeT X8>
  // generic swizzle
  inline PackedFloat8_256 swizzle(const PackedFloat8_256 &values) noexcept {
    static_assert(X1 < VectorSizes::X8, "Index X1 is out of range");
    static_assert(X2 < VectorSizes::X8, "Index X2 is out of range");
    static_assert(X3 < VectorSizes::X8, "Index X3 is out of range");
    static_assert(X4 < VectorSizes::X8, "Index X4 is out of range");
    static_assert(X5 < VectorSizes::X8, "Index X5 is out of range");
    static_assert(X6 < VectorSizes::X8, "Index X6 is out of range");
    static_assert(X7 < VectorSizes::X8, "Index X7 is out of range");
    static_assert(X8 < VectorSizes::X8, "Index X8 is out of range");

    // permute components into the expected slots by permuting each PackedFloat2_128 individually
    const auto swizzledX1toX4 =
        swizzle<X1 & VectorIndices::X4, X2 & VectorIndices::X4, X3 & VectorIndices::X4, X4 & VectorIndices::X4>(values);

    const auto swizzledX5toX8 =
        swizzle<X5 & VectorIndices::X4, X6 & VectorIndices::X4, X7 & VectorIndices::X4, X8 & VectorIndices::X4>(values);

    // merge X1toX4 and X5toX8 together for Xi < VectorIndices::X5
    const auto swizzledLo = _mm256_permute2f128_ps(swizzledX1toX4, swizzledX5toX8, 0b00100000);

    // merge Hi X1toX4 and X5toX8 together for Xi >= VectorIndices::X5
    const auto swizzledHi = _mm256_permute2f128_ps(swizzledX1toX4, swizzledX5toX8, 0b00110001);

    // blend both full 256 bit according to the original indices
    return blend<
        (X1 >= VectorIndices::X5), (X2 >= VectorIndices::X5), (X3 >= VectorIndices::X5), (X4 >= VectorIndices::X5),
        (X5 >= VectorIndices::X5), (X6 >= VectorIndices::X5), (X7 >= VectorIndices::X5), (X8 >= VectorIndices::X5)>(
        swizzledLo, swizzledHi
    );
  }

  template <>
  // passthrough swizzle
  PackedFloat8_256 swizzle<
      VectorIndices::X1, VectorIndices::X2, VectorIndices::X3, VectorIndices::X4, VectorIndices::X5, VectorIndices::X6,
      VectorIndices::X7, VectorIndices::X8>(const PackedFloat8_256 &values) noexcept;

  template <>
  // unpack lo index components on each PackedFloat4_128
  PackedFloat8_256 swizzle<
      VectorIndices::X1, VectorIndices::X1, VectorIndices::X2, VectorIndices::X2, VectorIndices::X5, VectorIndices::X5,
      VectorIndices::X6, VectorIndices::X6>(const PackedFloat8_256 &values) noexcept;

  template <>
  // duplicate odd-index components
  PackedFloat8_256 swizzle<
      VectorIndices::X1, VectorIndices::X1, VectorIndices::X3, VectorIndices::X3, VectorIndices::X5, VectorIndices::X5,
      VectorIndices::X7, VectorIndices::X7>(const PackedFloat8_256 &values) noexcept;

  template <>
  // duplicate lo PackedFloat4_128
  PackedFloat8_256 swizzle<
      VectorIndices::X1, VectorIndices::X2, VectorIndices::X3, VectorIndices::X4, VectorIndices::X1, VectorIndices::X2,
      VectorIndices::X3, VectorIndices::X4>(const PackedFloat8_256 &values) noexcept;

  template <>
  // duplicate even-index components
  PackedFloat8_256 swizzle<
      VectorIndices::X2, VectorIndices::X2, VectorIndices::X4, VectorIndices::X4, VectorIndices::X6, VectorIndices::X6,
      VectorIndices::X8, VectorIndices::X8>(const PackedFloat8_256 &values) noexcept;

  template <>
  // unpack hi index components on each PackedFloat4_128
  PackedFloat8_256 swizzle<
      VectorIndices::X3, VectorIndices::X3, VectorIndices::X4, VectorIndices::X4, VectorIndices::X7, VectorIndices::X7,
      VectorIndices::X8, VectorIndices::X8>(const PackedFloat8_256 &values) noexcept;

  template <>
  // swap PackedFloat4_128
  PackedFloat8_256 swizzle<
      VectorIndices::X5, VectorIndices::X6, VectorIndices::X7, VectorIndices::X8, VectorIndices::X1, VectorIndices::X2,
      VectorIndices::X3, VectorIndices::X4>(const PackedFloat8_256 &values) noexcept;

  template <>
  // duplicate hi PackedFloat4_128
  PackedFloat8_256 swizzle<
      VectorIndices::X5, VectorIndices::X6, VectorIndices::X7, VectorIndices::X8, VectorIndices::X5, VectorIndices::X6,
      VectorIndices::X7, VectorIndices::X8>(const PackedFloat8_256 &values) noexcept;
}
