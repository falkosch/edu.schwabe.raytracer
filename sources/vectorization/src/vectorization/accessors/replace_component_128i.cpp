#include "vectorization/accessors/replace_component_128i.h"

#include "vectorization/blends/blend_masked_128i.h"
#include "vectorization/functions/broadcast.h"

#include <cassert>

namespace vectorization {
  template <>
  PackedInts_128
  replaceComponent<VectorIndices::X>(const PackedInts_128 &values, const Int_32 replacement) noexcept {
    return _mm_insert_epi32(values, replacement, VectorIndices::X);
  }

  template <>
  PackedInts_128
  replaceComponent<VectorIndices::Y>(const PackedInts_128 &values, const Int_32 replacement) noexcept {
    return _mm_insert_epi32(values, replacement, VectorIndices::Y);
  }

  template <>
  PackedInts_128
  replaceComponent<VectorIndices::Z>(const PackedInts_128 &values, const Int_32 replacement) noexcept {
    return _mm_insert_epi32(values, replacement, VectorIndices::Z);
  }

  template <>
  PackedInts_128
  replaceComponent<VectorIndices::W>(const PackedInts_128 &values, const Int_32 replacement) noexcept {
    return _mm_insert_epi32(values, replacement, VectorIndices::W);
  }

  PackedInts_128 replaceX(const PackedInts_128 &values, const Int_32 replacement) noexcept {
    return replaceComponent<VectorIndices::X>(values, replacement);
  }

  PackedInts_128 replaceY(const PackedInts_128 &values, const Int_32 replacement) noexcept {
    return replaceComponent<VectorIndices::Y>(values, replacement);
  }

  PackedInts_128 replaceZ(const PackedInts_128 &values, const Int_32 replacement) noexcept {
    return replaceComponent<VectorIndices::Z>(values, replacement);
  }

  PackedInts_128 replaceW(const PackedInts_128 &values, const Int_32 replacement) noexcept {
    return replaceComponent<VectorIndices::W>(values, replacement);
  }

  PackedInts_128 replaceX1(const PackedInts_128 &values, const Int_32 replacement) noexcept {
    return replaceComponent<VectorIndices::X1>(values, replacement);
  }

  PackedInts_128 replaceX2(const PackedInts_128 &values, const Int_32 replacement) noexcept {
    return replaceComponent<VectorIndices::X2>(values, replacement);
  }

  PackedInts_128 replaceX3(const PackedInts_128 &values, const Int_32 replacement) noexcept {
    return replaceComponent<VectorIndices::X3>(values, replacement);
  }

  PackedInts_128 replaceX4(const PackedInts_128 &values, const Int_32 replacement) noexcept {
    return replaceComponent<VectorIndices::X4>(values, replacement);
  }

  PackedInts_128
  replaceComponent(const PackedInts_128 &values, const PackedInts_128 &replacement, const ASizeT index) noexcept {
    assert(index < VectorSizes::W);
    const auto indexBlendMask = _mm_cmpeq_epi32(broadcast(static_cast<Int_32>(index)), _mm_set_epi32(3, 2, 1, 0));
    return blendMasked(values, replacement, indexBlendMask);
  }

  PackedInts_128
  replaceComponent(const PackedInts_128 &values, const Int_32 replacement, const ASizeT index) noexcept {
    return replaceComponent(values, broadcast(replacement), index);
  }
}
