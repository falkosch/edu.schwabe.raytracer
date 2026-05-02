#pragma once

#include "vectorization/architecture.h"
#include "vectorization/blends/blend_256i.h"

namespace vectorization {
  template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W>
  PackedInts_256 swizzle(const PackedInts_256 &values) noexcept {
    static_assert(X < VectorSizes::W, "Index X is out of range");
    static_assert(Y < VectorSizes::W, "Index Y is out of range");
    static_assert(Z < VectorSizes::W, "Index Z is out of range");
    static_assert(W < VectorSizes::W, "Index W is out of range");

    constexpr auto PermuteMask = X | (Y << 2) | (Z << 4) | (W << 6);
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_shuffle_epi32(values, PermuteMask);
#else
    return _mm256_castps_si256(_mm256_permute_ps(_mm256_castsi256_ps(values), PermuteMask));
#endif
  }

  template <>
  PackedInts_256
  swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(const PackedInts_256 &values
  ) noexcept;

  template <>
  PackedInts_256
  swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y>(const PackedInts_256 &values
  ) noexcept;

  template <>
  PackedInts_256
  swizzle<VectorIndices::Y, VectorIndices::Y, VectorIndices::W, VectorIndices::W>(const PackedInts_256 &values
  ) noexcept;

  template <>
  PackedInts_256
  swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(const PackedInts_256 &values
  ) noexcept;

  template <>
  PackedInts_256
  swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(const PackedInts_256 &values
  ) noexcept;

  template <>
  PackedInts_256
  swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(const PackedInts_256 &values
  ) noexcept;

  PackedInts_256 xxxx(const PackedInts_256 &values) noexcept;
  PackedInts_256 yyyy(const PackedInts_256 &values) noexcept;
  PackedInts_256 zzzz(const PackedInts_256 &values) noexcept;
  PackedInts_256 wwww(const PackedInts_256 &values) noexcept;
  PackedInts_256 xxyy(const PackedInts_256 &values) noexcept;
  PackedInts_256 xyxy(const PackedInts_256 &values) noexcept;
  PackedInts_256 yyww(const PackedInts_256 &values) noexcept;
  PackedInts_256 zzww(const PackedInts_256 &values) noexcept;
  PackedInts_256 zwzw(const PackedInts_256 &values) noexcept;

  template <ASizeT X1, ASizeT X2, ASizeT X3, ASizeT X4, ASizeT X5, ASizeT X6, ASizeT X7, ASizeT X8>
  PackedInts_256 swizzle(const PackedInts_256 &values) noexcept {
    static_assert(X1 < VectorSizes::X8, "Index X1 is out of range");
    static_assert(X2 < VectorSizes::X8, "Index X2 is out of range");
    static_assert(X3 < VectorSizes::X8, "Index X3 is out of range");
    static_assert(X4 < VectorSizes::X8, "Index X4 is out of range");
    static_assert(X5 < VectorSizes::X8, "Index X5 is out of range");
    static_assert(X6 < VectorSizes::X8, "Index X6 is out of range");
    static_assert(X7 < VectorSizes::X8, "Index X7 is out of range");
    static_assert(X8 < VectorSizes::X8, "Index X8 is out of range");

    const auto swizzledX1toX4 =
        swizzle<X1 & VectorIndices::X4, X2 & VectorIndices::X4, X3 & VectorIndices::X4, X4 & VectorIndices::X4>(values);

    const auto swizzledX5toX8 =
        swizzle<X5 & VectorIndices::X4, X6 & VectorIndices::X4, X7 & VectorIndices::X4, X8 & VectorIndices::X4>(values);

    const auto swizzledLo = _mm256_permute2x128_si256(swizzledX1toX4, swizzledX5toX8, 0b00100000);
    const auto swizzledHi = _mm256_permute2x128_si256(swizzledX1toX4, swizzledX5toX8, 0b00110001);

    return blend<
        (X1 >= VectorIndices::X5), (X2 >= VectorIndices::X5), (X3 >= VectorIndices::X5), (X4 >= VectorIndices::X5),
        (X5 >= VectorIndices::X5), (X6 >= VectorIndices::X5), (X7 >= VectorIndices::X5), (X8 >= VectorIndices::X5)>(
        swizzledLo, swizzledHi
    );
  }

  template <>
  PackedInts_256 swizzle<
      VectorIndices::X1, VectorIndices::X2, VectorIndices::X3, VectorIndices::X4, VectorIndices::X5, VectorIndices::X6,
      VectorIndices::X7, VectorIndices::X8>(const PackedInts_256 &values) noexcept;

  template <>
  PackedInts_256 swizzle<
      VectorIndices::X5, VectorIndices::X6, VectorIndices::X7, VectorIndices::X8, VectorIndices::X1, VectorIndices::X2,
      VectorIndices::X3, VectorIndices::X4>(const PackedInts_256 &values) noexcept;

  template <>
  PackedInts_256 swizzle<
      VectorIndices::X1, VectorIndices::X2, VectorIndices::X3, VectorIndices::X4, VectorIndices::X1, VectorIndices::X2,
      VectorIndices::X3, VectorIndices::X4>(const PackedInts_256 &values) noexcept;

  template <>
  PackedInts_256 swizzle<
      VectorIndices::X5, VectorIndices::X6, VectorIndices::X7, VectorIndices::X8, VectorIndices::X5, VectorIndices::X6,
      VectorIndices::X7, VectorIndices::X8>(const PackedInts_256 &values) noexcept;
}
