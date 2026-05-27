#include "vectorization/functions/compare.h"

namespace vectorization {
  PackedFloat4_128 compareLess(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept {
    return _mm_cmplt_ps(a, b);
  }

  PackedFloat2_128 compareLess(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept {
    return _mm_cmplt_pd(a, b);
  }

  PackedFloat8_256 compareLess(const PackedFloat8_256 &a, const PackedFloat8_256 &b) noexcept {
    return _mm256_cmp_ps(a, b, _CMP_LT_OS);
  }

  PackedFloat4_256 compareLess(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept {
    return _mm256_cmp_pd(a, b, _CMP_LT_OS);
  }

  PackedFloat4_128 compareGreater(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept {
    return _mm_cmpgt_ps(a, b);
  }

  PackedFloat2_128 compareGreater(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept {
    return _mm_cmpgt_pd(a, b);
  }

  PackedFloat8_256 compareGreater(const PackedFloat8_256 &a, const PackedFloat8_256 &b) noexcept {
    return _mm256_cmp_ps(a, b, _CMP_GT_OS);
  }

  PackedFloat4_256 compareGreater(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept {
    return _mm256_cmp_pd(a, b, _CMP_GT_OS);
  }

  PackedFloat4_128 compareLessEqual(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept {
    return _mm_cmple_ps(a, b);
  }

  PackedFloat2_128 compareLessEqual(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept {
    return _mm_cmple_pd(a, b);
  }

  PackedFloat8_256 compareLessEqual(const PackedFloat8_256 &a, const PackedFloat8_256 &b) noexcept {
    return _mm256_cmp_ps(a, b, _CMP_LE_OS);
  }

  PackedFloat4_256 compareLessEqual(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept {
    return _mm256_cmp_pd(a, b, _CMP_LE_OS);
  }

  PackedFloat4_128 compareGreaterEqual(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept {
    return _mm_cmpge_ps(a, b);
  }

  PackedFloat2_128 compareGreaterEqual(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept {
    return _mm_cmpge_pd(a, b);
  }

  PackedFloat8_256 compareGreaterEqual(const PackedFloat8_256 &a, const PackedFloat8_256 &b) noexcept {
    return _mm256_cmp_ps(a, b, _CMP_GE_OS);
  }

  PackedFloat4_256 compareGreaterEqual(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept {
    return _mm256_cmp_pd(a, b, _CMP_GE_OS);
  }

  PackedFloat4_128 compareEqual(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept {
    return _mm_cmpeq_ps(a, b);
  }

  PackedFloat2_128 compareEqual(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept {
    return _mm_cmpeq_pd(a, b);
  }

  PackedFloat8_256 compareEqual(const PackedFloat8_256 &a, const PackedFloat8_256 &b) noexcept {
    return _mm256_cmp_ps(a, b, _CMP_EQ_OQ);
  }

  PackedFloat4_256 compareEqual(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept {
    return _mm256_cmp_pd(a, b, _CMP_EQ_OQ);
  }

  PackedFloat4_128 compareNotEqual(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept {
    return _mm_cmpneq_ps(a, b);
  }

  PackedFloat2_128 compareNotEqual(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept {
    return _mm_cmpneq_pd(a, b);
  }

  PackedFloat8_256 compareNotEqual(const PackedFloat8_256 &a, const PackedFloat8_256 &b) noexcept {
    return _mm256_cmp_ps(a, b, _CMP_NEQ_UQ);
  }

  PackedFloat4_256 compareNotEqual(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept {
    return _mm256_cmp_pd(a, b, _CMP_NEQ_UQ);
  }
}
