#include "vectorization/accessors/replaceComponent_256d.h"

#include "vectorization/blends.h"

#include <cassert>

namespace vectorization {
  template <>
  PackedFloat4_256
  replaceComponent<VectorIndices::X>(const PackedFloat4_256 &values, const Float_64 replacement) noexcept {
    const auto broadcast = _mm256_set1_pd(replacement);
    return blend<true, false, false, false>(values, broadcast);
  }

  template <>
  PackedFloat4_256
  replaceComponent<VectorIndices::Y>(const PackedFloat4_256 &values, const Float_64 replacement) noexcept {
    const auto broadcast = _mm256_set1_pd(replacement);
    return blend<false, true, false, false>(values, broadcast);
  }

  template <>
  PackedFloat4_256
  replaceComponent<VectorIndices::Z>(const PackedFloat4_256 &values, const Float_64 replacement) noexcept {
    const auto broadcast = _mm256_set1_pd(replacement);
    return blend<false, false, true, false>(values, broadcast);
  }

  template <>
  PackedFloat4_256
  replaceComponent<VectorIndices::W>(const PackedFloat4_256 &values, const Float_64 replacement) noexcept {
    const auto broadcast = _mm256_set1_pd(replacement);
    return blend<false, false, false, true>(values, broadcast);
  }

  PackedFloat4_256 replaceX(const PackedFloat4_256 &values, const Float_64 replacement) noexcept {
    return replaceComponent<VectorIndices::X>(values, replacement);
  }

  PackedFloat4_256 replaceY(const PackedFloat4_256 &values, const Float_64 replacement) noexcept {
    return replaceComponent<VectorIndices::Y>(values, replacement);
  }

  PackedFloat4_256 replaceZ(const PackedFloat4_256 &values, const Float_64 replacement) noexcept {
    return replaceComponent<VectorIndices::Z>(values, replacement);
  }

  PackedFloat4_256 replaceW(const PackedFloat4_256 &values, const Float_64 replacement) noexcept {
    return replaceComponent<VectorIndices::W>(values, replacement);
  }

  PackedFloat4_256 replaceX1(const PackedFloat4_256 &values, const Float_64 replacement) noexcept {
    return replaceComponent<VectorIndices::X1>(values, replacement);
  }

  PackedFloat4_256 replaceX2(const PackedFloat4_256 &values, const Float_64 replacement) noexcept {
    return replaceComponent<VectorIndices::X2>(values, replacement);
  }

  PackedFloat4_256 replaceX3(const PackedFloat4_256 &values, const Float_64 replacement) noexcept {
    return replaceComponent<VectorIndices::X3>(values, replacement);
  }

  PackedFloat4_256 replaceX4(const PackedFloat4_256 &values, const Float_64 replacement) noexcept {
    return replaceComponent<VectorIndices::X4>(values, replacement);
  }

  PackedFloat4_256
  replaceComponent(const PackedFloat4_256 &values, const PackedFloat4_256 &replacement, const ASizeT index) noexcept {
    assert(index < VectorSizes::Y);
    const auto mask = _mm256_cmpeq_epi64(_mm256_set1_epi64x(static_cast<int>(index)), _mm256_set_epi64x(3, 2, 1, 0));
    return blendMasked(values, replacement, mask);
  }

  PackedFloat4_256
  replaceComponent(const PackedFloat4_256 &values, const Float_64 replacement, const ASizeT index) noexcept {
    assert(index < VectorSizes::Y);
    return replaceComponent(values, _mm256_set1_pd(replacement), index);
  }
}
