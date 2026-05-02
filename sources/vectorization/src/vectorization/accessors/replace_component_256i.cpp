#include "vectorization/accessors/replace_component_256i.h"

#include "vectorization/blends/blend_256i.h"
#include "vectorization/blends/blend_masked_256i.h"

#include <cassert>

namespace vectorization {
  template <>
  PackedInts_256
  replaceComponent<VectorIndices::X1>(const PackedInts_256 &values, const Int_32 replacement) noexcept {
    const auto broadcast = _mm256_set1_epi32(replacement);
    return blend<true, false, false, false, false, false, false, false>(values, broadcast);
  }

  template <>
  PackedInts_256
  replaceComponent<VectorIndices::X2>(const PackedInts_256 &values, const Int_32 replacement) noexcept {
    const auto broadcast = _mm256_set1_epi32(replacement);
    return blend<false, true, false, false, false, false, false, false>(values, broadcast);
  }

  template <>
  PackedInts_256
  replaceComponent<VectorIndices::X3>(const PackedInts_256 &values, const Int_32 replacement) noexcept {
    const auto broadcast = _mm256_set1_epi32(replacement);
    return blend<false, false, true, false, false, false, false, false>(values, broadcast);
  }

  template <>
  PackedInts_256
  replaceComponent<VectorIndices::X4>(const PackedInts_256 &values, const Int_32 replacement) noexcept {
    const auto broadcast = _mm256_set1_epi32(replacement);
    return blend<false, false, false, true, false, false, false, false>(values, broadcast);
  }

  template <>
  PackedInts_256
  replaceComponent<VectorIndices::X5>(const PackedInts_256 &values, const Int_32 replacement) noexcept {
    const auto broadcast = _mm256_set1_epi32(replacement);
    return blend<false, false, false, false, true, false, false, false>(values, broadcast);
  }

  template <>
  PackedInts_256
  replaceComponent<VectorIndices::X6>(const PackedInts_256 &values, const Int_32 replacement) noexcept {
    const auto broadcast = _mm256_set1_epi32(replacement);
    return blend<false, false, false, false, false, true, false, false>(values, broadcast);
  }

  template <>
  PackedInts_256
  replaceComponent<VectorIndices::X7>(const PackedInts_256 &values, const Int_32 replacement) noexcept {
    const auto broadcast = _mm256_set1_epi32(replacement);
    return blend<false, false, false, false, false, false, true, false>(values, broadcast);
  }

  template <>
  PackedInts_256
  replaceComponent<VectorIndices::X8>(const PackedInts_256 &values, const Int_32 replacement) noexcept {
    const auto broadcast = _mm256_set1_epi32(replacement);
    return blend<false, false, false, false, false, false, false, true>(values, broadcast);
  }

  PackedInts_256 replaceX1(const PackedInts_256 &values, const Int_32 replacement) noexcept {
    return replaceComponent<VectorIndices::X1>(values, replacement);
  }

  PackedInts_256 replaceX2(const PackedInts_256 &values, const Int_32 replacement) noexcept {
    return replaceComponent<VectorIndices::X2>(values, replacement);
  }

  PackedInts_256 replaceX3(const PackedInts_256 &values, const Int_32 replacement) noexcept {
    return replaceComponent<VectorIndices::X3>(values, replacement);
  }

  PackedInts_256 replaceX4(const PackedInts_256 &values, const Int_32 replacement) noexcept {
    return replaceComponent<VectorIndices::X4>(values, replacement);
  }

  PackedInts_256 replaceX5(const PackedInts_256 &values, const Int_32 replacement) noexcept {
    return replaceComponent<VectorIndices::X5>(values, replacement);
  }

  PackedInts_256 replaceX6(const PackedInts_256 &values, const Int_32 replacement) noexcept {
    return replaceComponent<VectorIndices::X6>(values, replacement);
  }

  PackedInts_256 replaceX7(const PackedInts_256 &values, const Int_32 replacement) noexcept {
    return replaceComponent<VectorIndices::X7>(values, replacement);
  }

  PackedInts_256 replaceX8(const PackedInts_256 &values, const Int_32 replacement) noexcept {
    return replaceComponent<VectorIndices::X8>(values, replacement);
  }

  PackedInts_256
  replaceComponent(const PackedInts_256 &values, const PackedInts_256 &replacement, const ASizeT index) noexcept {
    assert(index < VectorSizes::X8);
    const auto indexBlendMask =
        _mm256_cmpeq_epi32(_mm256_set1_epi32(static_cast<int>(index)), _mm256_set_epi32(7, 6, 5, 4, 3, 2, 1, 0));
    return blendMasked(values, replacement, indexBlendMask);
  }

  PackedInts_256
  replaceComponent(const PackedInts_256 &values, const Int_32 replacement, const ASizeT index) noexcept {
    return replaceComponent(values, _mm256_set1_epi32(replacement), index);
  }
}
