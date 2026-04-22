#include "vectorization/accessors/component_128i.h"

namespace vectorization {
  template <>
  Int_32 component<VectorIndices::X>(const PackedInts_128 &values) noexcept {
    return _mm_cvtsi128_si32(values);
  }

  template <>
  Int_32 component<VectorIndices::Y>(const PackedInts_128 &values) noexcept {
    return _mm_extract_epi32(values, VectorIndices::Y);
  }

  template <>
  Int_32 component<VectorIndices::Z>(const PackedInts_128 &values) noexcept {
    return _mm_extract_epi32(values, VectorIndices::Z);
  }

  template <>
  Int_32 component<VectorIndices::W>(const PackedInts_128 &values) noexcept {
    return _mm_extract_epi32(values, VectorIndices::W);
  }

  Int_32 x(const PackedInts_128 &values) noexcept {
    return component<VectorIndices::X>(values);
  }

  Int_32 y(const PackedInts_128 &values) noexcept {
    return component<VectorIndices::Y>(values);
  }

  Int_32 z(const PackedInts_128 &values) noexcept {
    return component<VectorIndices::Z>(values);
  }

  Int_32 w(const PackedInts_128 &values) noexcept {
    return component<VectorIndices::W>(values);
  }

  Int_32 x1(const PackedInts_128 &values) noexcept {
    return component<VectorIndices::X1>(values);
  }

  Int_32 x2(const PackedInts_128 &values) noexcept {
    return component<VectorIndices::X2>(values);
  }

  Int_32 x3(const PackedInts_128 &values) noexcept {
    return component<VectorIndices::X3>(values);
  }

  Int_32 x4(const PackedInts_128 &values) noexcept {
    return component<VectorIndices::X4>(values);
  }
}
