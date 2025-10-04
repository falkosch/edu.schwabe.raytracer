#include "vectorization/accessors/replaceComponent_256s.h"

#include "vectorization/blends.h"

#include <cassert>

namespace vectorization {
  template <>
  PackedFloat8_256
  replaceComponent<VectorIndices::X1>(const PackedFloat8_256 &values, const Float_32 replacement) noexcept {
    const auto broadcast = _mm256_set1_ps(replacement);
    return blend<true, false, false, false, false, false, false, false>(values, broadcast);
  }

  template <>
  PackedFloat8_256
  replaceComponent<VectorIndices::X2>(const PackedFloat8_256 &values, const Float_32 replacement) noexcept {
    const auto broadcast = _mm256_set1_ps(replacement);
    return blend<false, true, false, false, false, false, false, false>(values, broadcast);
  }

  template <>
  PackedFloat8_256
  replaceComponent<VectorIndices::X3>(const PackedFloat8_256 &values, const Float_32 replacement) noexcept {
    const auto broadcast = _mm256_set1_ps(replacement);
    return blend<false, false, true, false, false, false, false, false>(values, broadcast);
  }

  template <>
  PackedFloat8_256
  replaceComponent<VectorIndices::X4>(const PackedFloat8_256 &values, const Float_32 replacement) noexcept {
    const auto broadcast = _mm256_set1_ps(replacement);
    return blend<false, false, false, true, false, false, false, false>(values, broadcast);
  }

  template <>
  PackedFloat8_256
  replaceComponent<VectorIndices::X5>(const PackedFloat8_256 &values, const Float_32 replacement) noexcept {
    const auto broadcast = _mm256_set1_ps(replacement);
    return blend<false, false, false, false, true, false, false, false>(values, broadcast);
  }

  template <>
  PackedFloat8_256
  replaceComponent<VectorIndices::X6>(const PackedFloat8_256 &values, const Float_32 replacement) noexcept {
    const auto broadcast = _mm256_set1_ps(replacement);
    return blend<false, false, false, false, false, true, false, false>(values, broadcast);
  }

  template <>
  PackedFloat8_256
  replaceComponent<VectorIndices::X7>(const PackedFloat8_256 &values, const Float_32 replacement) noexcept {
    const auto broadcast = _mm256_set1_ps(replacement);
    return blend<false, false, false, false, false, false, true, false>(values, broadcast);
  }

  template <>
  PackedFloat8_256
  replaceComponent<VectorIndices::X8>(const PackedFloat8_256 &values, const Float_32 replacement) noexcept {
    const auto broadcast = _mm256_set1_ps(replacement);
    return blend<false, false, false, false, false, false, false, true>(values, broadcast);
  }

  PackedFloat8_256 replaceX(const PackedFloat8_256 &values, const Float_32 replacement) noexcept {
    return replaceComponent<VectorIndices::X>(values, replacement);
  }

  PackedFloat8_256 replaceY(const PackedFloat8_256 &values, const Float_32 replacement) noexcept {
    return replaceComponent<VectorIndices::Y>(values, replacement);
  }

  PackedFloat8_256 replaceZ(const PackedFloat8_256 &values, const Float_32 replacement) noexcept {
    return replaceComponent<VectorIndices::Z>(values, replacement);
  }

  PackedFloat8_256 replaceW(const PackedFloat8_256 &values, const Float_32 replacement) noexcept {
    return replaceComponent<VectorIndices::W>(values, replacement);
  }

  PackedFloat8_256 replaceX1(const PackedFloat8_256 &values, const Float_32 replacement) noexcept {
    return replaceComponent<VectorIndices::X1>(values, replacement);
  }

  PackedFloat8_256 replaceX2(const PackedFloat8_256 &values, const Float_32 replacement) noexcept {
    return replaceComponent<VectorIndices::X2>(values, replacement);
  }

  PackedFloat8_256 replaceX3(const PackedFloat8_256 &values, const Float_32 replacement) noexcept {
    return replaceComponent<VectorIndices::X3>(values, replacement);
  }

  PackedFloat8_256 replaceX4(const PackedFloat8_256 &values, const Float_32 replacement) noexcept {
    return replaceComponent<VectorIndices::X4>(values, replacement);
  }

  PackedFloat8_256 replaceX5(const PackedFloat8_256 &values, const Float_32 replacement) noexcept {
    return replaceComponent<VectorIndices::X5>(values, replacement);
  }

  PackedFloat8_256 replaceX6(const PackedFloat8_256 &values, const Float_32 replacement) noexcept {
    return replaceComponent<VectorIndices::X6>(values, replacement);
  }

  PackedFloat8_256 replaceX7(const PackedFloat8_256 &values, const Float_32 replacement) noexcept {
    return replaceComponent<VectorIndices::X7>(values, replacement);
  }

  PackedFloat8_256 replaceX8(const PackedFloat8_256 &values, const Float_32 replacement) noexcept {
    return replaceComponent<VectorIndices::X8>(values, replacement);
  }

  PackedFloat8_256
  replaceComponent(const PackedFloat8_256 &values, const PackedFloat8_256 &replacement, const ASizeT index) noexcept {
    assert(index < VectorSizes::Y);
    const auto mask =
        _mm256_cmpeq_epi32(_mm256_set1_epi32(static_cast<int>(index)), _mm256_set_epi32(7, 6, 5, 4, 3, 2, 1, 0));
    return blendMasked(values, replacement, mask);
  }

  PackedFloat8_256
  replaceComponent(const PackedFloat8_256 &values, const Float_32 replacement, const ASizeT index) noexcept {
    return replaceComponent(values, _mm256_set1_ps(replacement), index);
  }
}
