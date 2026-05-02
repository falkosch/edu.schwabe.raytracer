#include "vectorization/swizzles/swizzle_256i.h"

namespace vectorization {
  template <>
  PackedInts_256
  swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(const PackedInts_256 &values
  ) noexcept {
    return values;
  }

  template <>
  PackedInts_256
  swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y>(const PackedInts_256 &values
  ) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_unpacklo_epi32(values, values);
#else
    const auto f = _mm256_castsi256_ps(values);
    return _mm256_castps_si256(_mm256_unpacklo_ps(f, f));
#endif
  }

  template <>
  PackedInts_256
  swizzle<VectorIndices::Y, VectorIndices::Y, VectorIndices::W, VectorIndices::W>(const PackedInts_256 &values
  ) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_shuffle_epi32(values, _MM_SHUFFLE(3, 3, 1, 1));
#else
    return _mm256_castps_si256(_mm256_permute_ps(_mm256_castsi256_ps(values), _MM_SHUFFLE(3, 3, 1, 1)));
#endif
  }

  template <>
  PackedInts_256
  swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(const PackedInts_256 &values
  ) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_unpackhi_epi32(values, values);
#else
    const auto f = _mm256_castsi256_ps(values);
    return _mm256_castps_si256(_mm256_unpackhi_ps(f, f));
#endif
  }

  template <>
  PackedInts_256
  swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(const PackedInts_256 &values
  ) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_unpacklo_epi64(values, values);
#else
    const auto d = _mm256_castsi256_pd(values);
    return _mm256_castpd_si256(_mm256_unpacklo_pd(d, d));
#endif
  }

  template <>
  PackedInts_256
  swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(const PackedInts_256 &values
  ) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_unpackhi_epi64(values, values);
#else
    const auto d = _mm256_castsi256_pd(values);
    return _mm256_castpd_si256(_mm256_unpackhi_pd(d, d));
#endif
  }

  PackedInts_256 xxxx(const PackedInts_256 &values) noexcept {
    return swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::X, VectorIndices::X>(values);
  }

  PackedInts_256 yyyy(const PackedInts_256 &values) noexcept {
    return swizzle<VectorIndices::Y, VectorIndices::Y, VectorIndices::Y, VectorIndices::Y>(values);
  }

  PackedInts_256 zzzz(const PackedInts_256 &values) noexcept {
    return swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::Z, VectorIndices::Z>(values);
  }

  PackedInts_256 wwww(const PackedInts_256 &values) noexcept {
    return swizzle<VectorIndices::W, VectorIndices::W, VectorIndices::W, VectorIndices::W>(values);
  }

  PackedInts_256 xxyy(const PackedInts_256 &values) noexcept {
    return swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y>(values);
  }

  PackedInts_256 xyxy(const PackedInts_256 &values) noexcept {
    return swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(values);
  }

  PackedInts_256 yyww(const PackedInts_256 &values) noexcept {
    return swizzle<VectorIndices::Y, VectorIndices::Y, VectorIndices::W, VectorIndices::W>(values);
  }

  PackedInts_256 zzww(const PackedInts_256 &values) noexcept {
    return swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(values);
  }

  PackedInts_256 zwzw(const PackedInts_256 &values) noexcept {
    return swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(values);
  }

  template <>
  PackedInts_256 swizzle<
      VectorIndices::X1, VectorIndices::X2, VectorIndices::X3, VectorIndices::X4, VectorIndices::X5, VectorIndices::X6,
      VectorIndices::X7, VectorIndices::X8>(const PackedInts_256 &values) noexcept {
    return values;
  }

  template <>
  PackedInts_256 swizzle<
      VectorIndices::X5, VectorIndices::X6, VectorIndices::X7, VectorIndices::X8, VectorIndices::X1, VectorIndices::X2,
      VectorIndices::X3, VectorIndices::X4>(const PackedInts_256 &values) noexcept {
    return _mm256_permute2x128_si256(values, values, 0b00000001);
  }

  template <>
  PackedInts_256 swizzle<
      VectorIndices::X1, VectorIndices::X2, VectorIndices::X3, VectorIndices::X4, VectorIndices::X1, VectorIndices::X2,
      VectorIndices::X3, VectorIndices::X4>(const PackedInts_256 &values) noexcept {
    return _mm256_permute2x128_si256(values, values, 0b00000000);
  }

  template <>
  PackedInts_256 swizzle<
      VectorIndices::X5, VectorIndices::X6, VectorIndices::X7, VectorIndices::X8, VectorIndices::X5, VectorIndices::X6,
      VectorIndices::X7, VectorIndices::X8>(const PackedInts_256 &values) noexcept {
    return _mm256_permute2x128_si256(values, values, 0b00010001);
  }
}
