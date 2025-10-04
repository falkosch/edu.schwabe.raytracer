#include "vectorization/accessors/replaceComponent_128s.h"

#include "vectorization/blends.h"

#include <cassert>

namespace vectorization {
  template <>
  PackedFloat4_128
  replaceComponent<VectorIndices::X>(const PackedFloat4_128 &values, const Float_32 replacement) noexcept {
    return _mm_insert_ps(
        values, _mm_set_ss(replacement), _MM_MK_INSERTPS_NDX(VectorIndices::X, VectorIndices::X, VectorBits::None)
    );
  }

  template <>
  PackedFloat4_128
  replaceComponent<VectorIndices::Y>(const PackedFloat4_128 &values, const Float_32 replacement) noexcept {
    return _mm_insert_ps(
        values, _mm_set_ss(replacement), _MM_MK_INSERTPS_NDX(VectorIndices::X, VectorIndices::Y, VectorBits::None)
    );
  }

  template <>
  PackedFloat4_128
  replaceComponent<VectorIndices::Z>(const PackedFloat4_128 &values, const Float_32 replacement) noexcept {
    return _mm_insert_ps(
        values, _mm_set_ss(replacement), _MM_MK_INSERTPS_NDX(VectorIndices::X, VectorIndices::Z, VectorBits::None)
    );
  }

  template <>
  PackedFloat4_128
  replaceComponent<VectorIndices::W>(const PackedFloat4_128 &values, const Float_32 replacement) noexcept {
    return _mm_insert_ps(
        values, _mm_set_ss(replacement), _MM_MK_INSERTPS_NDX(VectorIndices::X, VectorIndices::W, VectorBits::None)
    );
  }

  PackedFloat4_128 replaceX(const PackedFloat4_128 &values, const Float_32 replacement) noexcept {
    return replaceComponent<VectorIndices::X>(values, replacement);
  }

  PackedFloat4_128 replaceY(const PackedFloat4_128 &values, const Float_32 replacement) noexcept {
    return replaceComponent<VectorIndices::Y>(values, replacement);
  }

  PackedFloat4_128 replaceZ(const PackedFloat4_128 &values, const Float_32 replacement) noexcept {
    return replaceComponent<VectorIndices::Z>(values, replacement);
  }

  PackedFloat4_128 replaceW(const PackedFloat4_128 &values, const Float_32 replacement) noexcept {
    return replaceComponent<VectorIndices::W>(values, replacement);
  }

  PackedFloat4_128 replaceX1(const PackedFloat4_128 &values, const Float_32 replacement) noexcept {
    return replaceComponent<VectorIndices::X1>(values, replacement);
  }

  PackedFloat4_128 replaceX2(const PackedFloat4_128 &values, const Float_32 replacement) noexcept {
    return replaceComponent<VectorIndices::X2>(values, replacement);
  }

  PackedFloat4_128 replaceX3(const PackedFloat4_128 &values, const Float_32 replacement) noexcept {
    return replaceComponent<VectorIndices::X3>(values, replacement);
  }

  PackedFloat4_128 replaceX4(const PackedFloat4_128 &values, const Float_32 replacement) noexcept {
    return replaceComponent<VectorIndices::X4>(values, replacement);
  }

  PackedFloat4_128
  replaceComponent(const PackedFloat4_128 &values, const PackedFloat4_128 &replacement, const ASizeT index) noexcept {
    assert(index < VectorSizes::W);
    const auto indexBlendMask = _mm_cmpeq_epi32(_mm_set1_epi32(static_cast<int>(index)), _mm_set_epi32(3, 2, 1, 0));
    return blendMasked(values, replacement, indexBlendMask);
  }

  PackedFloat4_128
  replaceComponent(const PackedFloat4_128 &values, const Float_32 replacement, const ASizeT index) noexcept {
    return replaceComponent(values, _mm_set1_ps(replacement), index);
  }
}
