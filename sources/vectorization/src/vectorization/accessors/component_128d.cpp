#include "vectorization/accessors/component_128d.h"

#include "vectorization/swizzles.h"

namespace vectorization {
  template <>
  Float_64 component<VectorIndices::X>(const PackedFloat2_128 &values) noexcept {
    return _mm_cvtsd_f64(values);
  }

  template <>
  Float_64 component<VectorIndices::Y>(const PackedFloat2_128 &values) noexcept {
    return component<VectorIndices::X>(yy(values));
  }

  Float_64 x(const PackedFloat2_128 &values) noexcept {
    return component<VectorIndices::X>(values);
  }

  Float_64 y(const PackedFloat2_128 &values) noexcept {
    return component<VectorIndices::Y>(values);
  }

  Float_64 x1(const PackedFloat2_128 &values) noexcept {
    return component<VectorIndices::X1>(values);
  }

  Float_64 x2(const PackedFloat2_128 &values) noexcept {
    return component<VectorIndices::X2>(values);
  }
}
