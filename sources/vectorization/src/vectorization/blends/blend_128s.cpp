#include "vectorization/blends/blend_128s.h"

namespace vectorization {
  template <>
  PackedFloat4_128
  blend<false, false, false, false>(const PackedFloat4_128 &a, const PackedFloat4_128 & /*unused*/) noexcept {
    return a;
  }

  template <>
  PackedFloat4_128
  blend<true, true, true, true>(const PackedFloat4_128 & /*unused*/, const PackedFloat4_128 &b) noexcept {
    return b;
  }

  template <>
  PackedFloat4_128 blend<false, false, true, true>(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept {
    return _mm_shuffle_ps(a, b, _MM_SHUFFLE(VectorIndices::W, VectorIndices::Z, VectorIndices::Y, VectorIndices::X));
  }

  template <>
  PackedFloat4_128 blend<false, true, true, true>(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept {
    return _mm_move_ss(b, a);
  }

  template <>
  PackedFloat4_128 blend<true, false, false, false>(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept {
#pragma clang diagnostic push
#pragma ide diagnostic ignored "ArgumentSelectionDefects"
    // argument order is indeed (b, a)
    return blend<false, true, true, true>(b, a);
#pragma clang diagnostic pop
  }

  template <>
  PackedFloat4_128 blend<true, true, false, false>(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept {
#pragma clang diagnostic push
#pragma ide diagnostic ignored "ArgumentSelectionDefects"
    // argument order is indeed (b, a)
    return blend<false, false, true, true>(b, a);
#pragma clang diagnostic pop
  }

  PackedFloat4_128 x_yzw(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept {
    return blend<false, true, true, true>(a, b);
  }
}
