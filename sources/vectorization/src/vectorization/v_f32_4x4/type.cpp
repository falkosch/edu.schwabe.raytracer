#include "vectorization/v_f32_4x4.h"

namespace vectorization {
  v_f32_4x4::v_f32_4x4() noexcept = default;

  v_f32_4x4::v_f32_4x4(
      const PackedType &soaXIn, const PackedType &soaYIn, const PackedType &soaZIn, const PackedType &soaWIn
  ) noexcept
      : soaX(soaXIn), soaY(soaYIn), soaZ(soaZIn), soaW(soaWIn) {
  }

  v_f32_4x4::v_f32_4x4(
      const SOAVectorType &soaXIn, const SOAVectorType &soaYIn, const SOAVectorType &soaZIn, const SOAVectorType &soaWIn
  ) noexcept
      : soaX(soaXIn), soaY(soaYIn), soaZ(soaZIn), soaW(soaWIn) {
  }

  // broadcasting aos vector into its soa vectors
  v_f32_4x4::v_f32_4x4(const AOSVectorType &aos) noexcept {
    const auto pairXYXY = xyxy(aos);
    const auto pairZWZW = zwzw(aos);
    soaX = xxzz(pairXYXY);
    soaY = yyww(pairXYXY);
    soaZ = xxzz(pairZWZW);
    soaW = yyww(pairZWZW);
  }

  v_f32_4x4::v_f32_4x4(const ValueType *const values) noexcept
      : soaX(values + VectorIndices::X * SIZE_SOA), soaY(values + VectorIndices::Y * SIZE_SOA),
        soaZ(values + VectorIndices::Z * SIZE_SOA), soaW(values + VectorIndices::W * SIZE_SOA) {
  }
}
