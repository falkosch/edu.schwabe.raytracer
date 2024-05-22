#include "vectorization/accessors/replaceComponent_128d.h"

#include "vectorization/blends.h"

#include <cassert>

namespace vectorization {
  template <>
  PackedFloat2_128
  replaceComponent<VectorIndices::X>(const PackedFloat2_128 &values, const Float_64 replacement) noexcept {
    return _mm_shuffle_pd(_mm_set_sd(replacement), values, 0b10);
  }

  template <>
  PackedFloat2_128
  replaceComponent<VectorIndices::Y>(const PackedFloat2_128 &values, const Float_64 replacement) noexcept {
    return _mm_shuffle_pd(values, _mm_set_sd(replacement), 0b00);
  }

  PackedFloat2_128 replaceX(const PackedFloat2_128 &values, const Float_64 replacement) noexcept {
    return replaceComponent<VectorIndices::X>(values, replacement);
  }

  PackedFloat2_128 replaceY(const PackedFloat2_128 &values, const Float_64 replacement) noexcept {
    return replaceComponent<VectorIndices::Y>(values, replacement);
  }

  PackedFloat2_128 replaceX1(const PackedFloat2_128 &values, const Float_64 replacement) noexcept {
    return replaceComponent<VectorIndices::X1>(values, replacement);
  }

  PackedFloat2_128 replaceX2(const PackedFloat2_128 &values, const Float_64 replacement) noexcept {
    return replaceComponent<VectorIndices::X2>(values, replacement);
  }

  PackedFloat2_128
  replaceComponent(const PackedFloat2_128 &values, const PackedFloat2_128 &replacement, const ASizeT index) noexcept {
    assert(index < VectorSizes::Y);
    const auto mask = _mm_cmpeq_epi64(_mm_set1_epi64x(static_cast<int>(index)), _mm_set_epi64x(1, 0));
    return blendMasked(values, replacement, mask);
  }

  PackedFloat2_128
  replaceComponent(const PackedFloat2_128 &values, const Float_64 replacement, const ASizeT index) noexcept {
    return replaceComponent(values, _mm_set1_pd(replacement), index);
  }
}
