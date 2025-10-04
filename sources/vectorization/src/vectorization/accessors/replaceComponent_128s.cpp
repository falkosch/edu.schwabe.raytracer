#include "vectorization/accessors/replaceComponent_128s.h"

#include "vectorization/blends.h"

#include <cassert>

namespace vectorization {
  template <>
  const PackedFloat4_128 replaceComponent<VectorIndices::X>(const PackedFloat4_128 &v, const Float_32 s) noexcept {
    return _mm_insert_ps(v, _mm_set_ss(s), _MM_MK_INSERTPS_NDX(VectorIndices::X, VectorIndices::X, VectorBits::None));
  }

  template <>
  const PackedFloat4_128 replaceComponent<VectorIndices::Y>(const PackedFloat4_128 &v, const Float_32 s) noexcept {
    return _mm_insert_ps(v, _mm_set_ss(s), _MM_MK_INSERTPS_NDX(VectorIndices::X, VectorIndices::Y, VectorBits::None));
  }

  template <>
  const PackedFloat4_128 replaceComponent<VectorIndices::Z>(const PackedFloat4_128 &v, const Float_32 s) noexcept {
    return _mm_insert_ps(v, _mm_set_ss(s), _MM_MK_INSERTPS_NDX(VectorIndices::X, VectorIndices::Z, VectorBits::None));
  }

  template <>
  const PackedFloat4_128 replaceComponent<VectorIndices::W>(const PackedFloat4_128 &v, const Float_32 s) noexcept {
    return _mm_insert_ps(v, _mm_set_ss(s), _MM_MK_INSERTPS_NDX(VectorIndices::X, VectorIndices::W, VectorBits::None));
  }

  const PackedFloat4_128 replaceX(const PackedFloat4_128 &v, const Float_32 s) noexcept {
    return replaceComponent<VectorIndices::X>(v, s);
  }

  const PackedFloat4_128 replaceY(const PackedFloat4_128 &v, const Float_32 s) noexcept {
    return replaceComponent<VectorIndices::Y>(v, s);
  }

  const PackedFloat4_128 replaceZ(const PackedFloat4_128 &v, const Float_32 s) noexcept {
    return replaceComponent<VectorIndices::Z>(v, s);
  }

  const PackedFloat4_128 replaceW(const PackedFloat4_128 &v, const Float_32 s) noexcept {
    return replaceComponent<VectorIndices::W>(v, s);
  }

  const PackedFloat4_128 replaceX1(const PackedFloat4_128 &v, const Float_32 s) noexcept {
    return replaceComponent<VectorIndices::X1>(v, s);
  }

  const PackedFloat4_128 replaceX2(const PackedFloat4_128 &v, const Float_32 s) noexcept {
    return replaceComponent<VectorIndices::X2>(v, s);
  }

  const PackedFloat4_128 replaceX3(const PackedFloat4_128 &v, const Float_32 s) noexcept {
    return replaceComponent<VectorIndices::X3>(v, s);
  }

  const PackedFloat4_128 replaceX4(const PackedFloat4_128 &v, const Float_32 s) noexcept {
    return replaceComponent<VectorIndices::X4>(v, s);
  }

  const PackedFloat4_128
  replaceComponent(const PackedFloat4_128 &v, const PackedFloat4_128 &replacement, const ASizeT index) noexcept {
    assert(index < VectorSizes::W);
    auto indexBlendMask = _mm_cmpeq_epi32(_mm_set1_epi32(static_cast<int>(index)), _mm_set_epi32(3, 2, 1, 0));
    return blendMasked(v, replacement, indexBlendMask);
  }

  const PackedFloat4_128
  replaceComponent(const PackedFloat4_128 &v, const Float_32 replacement, const ASizeT index) noexcept {
    return replaceComponent(v, _mm_set1_ps(replacement), index);
  }
}