#include "vectorization/v_ui64_2.h"

namespace vectorization {
  v_ui64_2 min(const v_ui64_2 &a, const v_ui64_2 &b) noexcept {
    return blendMasked(a.components, b.components, (a > b).components);
  }

  v_ui64_2 minv(const v_ui64_2 &vector) noexcept {
    return min(vector, yy(vector));
  }

  v_ui64_2::ValueType min(const v_ui64_2 &vector) noexcept {
    return x(minv(vector));
  }

  v_ui64_2 max(const v_ui64_2 &a, const v_ui64_2 &b) noexcept {
    return blendMasked(b.components, a.components, (a > b).components);
  }

  v_ui64_2 maxv(const v_ui64_2 &vector) noexcept {
    return max(vector, yy(vector));
  }

  v_ui64_2::ValueType max(const v_ui64_2 &vector) noexcept {
    return x(maxv(vector));
  }

  v_ui64_2 lengthv(const v_ui64_2 &vector) noexcept {
    return xx(vector) + yy(vector);
  }

  v_ui64_2::ValueType length(const v_ui64_2 &vector) noexcept {
    return x(lengthv(vector));
  }

  v_ui64_2::ValueType distance(const v_ui64_2 &a, const v_ui64_2 &b) noexcept {
    return length(a - b);
  }

  v_ui64_2 clamp(const v_ui64_2 &vector, const v_ui64_2 &lowerVector, const v_ui64_2 &upperVector) noexcept {
    return max(lowerVector, min(vector, upperVector));
  }

  bool anyTrue(const v_ui64_2 &vector) noexcept {
    return !_mm_testz_si128(vector.components, MaskAll<v_ui64_2::PackedType>());
  }

  bool anyFalse(const v_ui64_2 &vector) noexcept {
    return !_mm_testc_si128(vector.components, MaskAll<v_ui64_2::PackedType>());
  }

  bool allTrue(const v_ui64_2 &vector) noexcept {
    return !!_mm_testc_si128(vector.components, MaskAll<v_ui64_2::PackedType>());
  }

  bool allFalse(const v_ui64_2 &vector) noexcept {
    return !!_mm_testz_si128(vector.components, MaskAll<v_ui64_2::PackedType>());
  }
}
