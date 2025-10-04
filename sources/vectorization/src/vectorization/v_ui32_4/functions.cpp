#include "vectorization/v_ui32_4.h"

namespace vectorization {
  v_ui32_4 zeroW(const v_ui32_4 &vector) noexcept {
    // Shift by four bytes left and then back right to erase the w-component
    return _mm_srli_si128(_mm_slli_si128(vector.components, sizeof(v_ui32_4::ValueType)), sizeof(v_ui32_4::ValueType));
  }

  v_ui32_4 lengthv(const v_ui32_4 &vector) noexcept {
    return xxxx(vector) + yyyy(vector) + zzzz(vector) + wwww(vector);
  }

  v_ui32_4 length3v(const v_ui32_4 &vector) noexcept {
    return xxxx(vector) + yyyy(vector) + zzzz(vector);
  }

  v_ui32_4::ValueType length(const v_ui32_4 &vector) noexcept {
    return x(lengthv(vector));
  }

  v_ui32_4::ValueType length3(const v_ui32_4 &vector) noexcept {
    return x(length3v(vector));
  }

  v_ui32_4::ValueType distance(const v_ui32_4 &a, const v_ui32_4 &b) noexcept {
    return length(a - b);
  }

  v_ui32_4::ValueType distance3(const v_ui32_4 &a, const v_ui32_4 &b) noexcept {
    return length3(a - b);
  }

  v_ui32_4 min(const v_ui32_4 &a, const v_ui32_4 &b) noexcept {
    return _mm_min_epu32(a.components, b.components);
  }

  v_ui32_4::ValueType min(const v_ui32_4 &vector) noexcept {
    // gets the min of this (x, y, z, w) and shuffled (z, w, z, w) and then
    // gets the min of XZ_YW (x', y', z', w') and its shuffled (y', x', w', z')
    const v_ui32_4 XZ_YW = min(vector, zwzw(vector));
    return x(min(XZ_YW, yxwz(XZ_YW)));
  }

  v_ui32_4::ValueType min3(const v_ui32_4 &vector) noexcept {
    // gets the min of (x, y, z, w), (z, w, z, w) and then of (y, x, w, z)
    return x(min(yxwz(vector), min(vector, zwzw(vector))));
  }

  v_ui32_4 max(const v_ui32_4 &a, const v_ui32_4 &b) noexcept {
    return _mm_max_epu32(a.components, b.components);
  }

  v_ui32_4::ValueType max(const v_ui32_4 &vector) noexcept {
    const v_ui32_4 XZ_YW = max(vector, zwzw(vector));
    return x(max(XZ_YW, yxwz(XZ_YW)));
  }

  v_ui32_4::ValueType max3(const v_ui32_4 &vector) noexcept {
    return x(max(yxwz(vector), max(vector, zwzw(vector))));
  }

  v_ui32_4 clamp(const v_ui32_4 &vector, const v_ui32_4 &lowerVector, const v_ui32_4 &upperVector) noexcept {
    return max(lowerVector, min(vector, upperVector));
  }

  bool anyTrue(const v_ui32_4 &vector) noexcept {
    return !_mm_testz_si128(vector.components, MaskAll<v_ui32_4::PackedType>());
  }

  bool anyTrue3(const v_ui32_4 &vector) noexcept {
    return !_mm_testz_si128(vector.components, MaskXYZ<v_ui32_4>().components);
  }

  bool anyFalse(const v_ui32_4 &vector) noexcept {
    return !_mm_testc_si128(vector.components, MaskAll<v_ui32_4::PackedType>());
  }

  bool anyFalse3(const v_ui32_4 &vector) noexcept {
    return !_mm_testc_si128(vector.components, MaskXYZ<v_ui32_4>().components);
  }

  bool allTrue(const v_ui32_4 &vector) noexcept {
    return !!_mm_testc_si128(vector.components, MaskAll<v_ui32_4::PackedType>());
  }

  bool allTrue3(const v_ui32_4 &vector) noexcept {
    return !!_mm_testc_si128(vector.components, MaskXYZ<v_ui32_4>().components);
  }

  bool allFalse(const v_ui32_4 &vector) noexcept {
    return !!_mm_testz_si128(vector.components, MaskAll<v_ui32_4::PackedType>());
  }

  bool allFalse3(const v_ui32_4 &vector) noexcept {
    return !!_mm_testz_si128(vector.components, MaskXYZ<v_ui32_4>().components);
  }
}
