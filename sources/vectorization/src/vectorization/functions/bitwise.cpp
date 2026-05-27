#include "vectorization/functions/bitwise.h"

#include "vectorization/constants/masks.h"

namespace vectorization {
  PackedFloat4_128 bitwiseNot(const PackedFloat4_128 &v) noexcept {
    return _mm_xor_ps(MaskAll<PackedFloat4_128>(), v);
  }

  PackedFloat2_128 bitwiseNot(const PackedFloat2_128 &v) noexcept {
    return _mm_xor_pd(MaskAll<PackedFloat2_128>(), v);
  }

  PackedFloat8_256 bitwiseNot(const PackedFloat8_256 &v) noexcept {
    return _mm256_xor_ps(MaskAll<PackedFloat8_256>(), v);
  }

  PackedFloat4_256 bitwiseNot(const PackedFloat4_256 &v) noexcept {
    return _mm256_xor_pd(MaskAll<PackedFloat4_256>(), v);
  }

  PackedInts_128 bitwiseNot(const PackedInts_128 &v) noexcept {
    return _mm_xor_si128(v, MaskAll<PackedInts_128>());
  }

  PackedInts_256 bitwiseNot(const PackedInts_256 &v) noexcept {
    return _mm256_xor_si256(v, MaskAll<PackedInts_256>());
  }

  // bitwiseAnd
  PackedFloat4_128 bitwiseAnd(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept {
    return _mm_and_ps(a, b);
  }

  PackedFloat2_128 bitwiseAnd(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept {
    return _mm_and_pd(a, b);
  }

  PackedFloat8_256 bitwiseAnd(const PackedFloat8_256 &a, const PackedFloat8_256 &b) noexcept {
    return _mm256_and_ps(a, b);
  }

  PackedFloat4_256 bitwiseAnd(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept {
    return _mm256_and_pd(a, b);
  }

  PackedInts_128 bitwiseAnd(const PackedInts_128 &a, const PackedInts_128 &b) noexcept {
    return _mm_and_si128(a, b);
  }

  PackedInts_256 bitwiseAnd(const PackedInts_256 &a, const PackedInts_256 &b) noexcept {
    return _mm256_and_si256(a, b);
  }

  // bitwiseOr
  PackedFloat4_128 bitwiseOr(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept {
    return _mm_or_ps(a, b);
  }

  PackedFloat2_128 bitwiseOr(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept {
    return _mm_or_pd(a, b);
  }

  PackedFloat8_256 bitwiseOr(const PackedFloat8_256 &a, const PackedFloat8_256 &b) noexcept {
    return _mm256_or_ps(a, b);
  }

  PackedFloat4_256 bitwiseOr(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept {
    return _mm256_or_pd(a, b);
  }

  PackedInts_128 bitwiseOr(const PackedInts_128 &a, const PackedInts_128 &b) noexcept {
    return _mm_or_si128(a, b);
  }

  PackedInts_256 bitwiseOr(const PackedInts_256 &a, const PackedInts_256 &b) noexcept {
    return _mm256_or_si256(a, b);
  }

  // bitwiseXor
  PackedFloat4_128 bitwiseXor(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept {
    return _mm_xor_ps(a, b);
  }

  PackedFloat2_128 bitwiseXor(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept {
    return _mm_xor_pd(a, b);
  }

  PackedFloat8_256 bitwiseXor(const PackedFloat8_256 &a, const PackedFloat8_256 &b) noexcept {
    return _mm256_xor_ps(a, b);
  }

  PackedFloat4_256 bitwiseXor(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept {
    return _mm256_xor_pd(a, b);
  }

  PackedInts_128 bitwiseXor(const PackedInts_128 &a, const PackedInts_128 &b) noexcept {
    return _mm_xor_si128(a, b);
  }

  PackedInts_256 bitwiseXor(const PackedInts_256 &a, const PackedInts_256 &b) noexcept {
    return _mm256_xor_si256(a, b);
  }

  // bitwiseAndNot: (~a) & b
  PackedFloat4_128 bitwiseAndNot(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept {
    return _mm_andnot_ps(a, b);
  }

  PackedFloat2_128 bitwiseAndNot(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept {
    return _mm_andnot_pd(a, b);
  }

  PackedFloat8_256 bitwiseAndNot(const PackedFloat8_256 &a, const PackedFloat8_256 &b) noexcept {
    return _mm256_andnot_ps(a, b);
  }

  PackedFloat4_256 bitwiseAndNot(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept {
    return _mm256_andnot_pd(a, b);
  }

  PackedInts_128 bitwiseAndNot(const PackedInts_128 &a, const PackedInts_128 &b) noexcept {
    return _mm_andnot_si128(a, b);
  }

  PackedInts_256 bitwiseAndNot(const PackedInts_256 &a, const PackedInts_256 &b) noexcept {
    return _mm256_andnot_si256(a, b);
  }
}
