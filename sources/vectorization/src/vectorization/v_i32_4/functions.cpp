#include "vectorization/v_i32_4.h"

namespace vectorization {
#pragma region length()
  //{ length()

  v_i32_4 lengthv(const v_i32_4 &v) noexcept {
    const v_i32_4 t = abs(v);
    return xxxx(t) + yyyy(t) + zzzz(t) + wwww(t);
  }

  v_i32_4 length3v(const v_i32_4 &v) noexcept {
    const v_i32_4 t = abs(v);
    return xxxx(t) + yyyy(t) + zzzz(t);
  }

  v_i32_4::ValueType length(const v_i32_4 &v) noexcept {
    return x(lengthv(v));
  }

  v_i32_4::ValueType length3(const v_i32_4 &v) noexcept {
    return x(length3v(v));
  }

  //}
#pragma endregion

#pragma region distance()
  //{ distance()

  v_i32_4::ValueType distance(const v_i32_4 &a, const v_i32_4 &b) noexcept {
    return length(b - a);
  }

  v_i32_4::ValueType distance3(const v_i32_4 &a, const v_i32_4 &b) noexcept {
    return length3(b - a);
  }

  //}
#pragma endregion

#pragma region min()
  //{ min()

  v_i32_4 min(const v_i32_4 &a, const v_i32_4 &b) noexcept {
    return _mm_min_epi32(a.components, b.components);
  }

  v_i32_4::ValueType min(const v_i32_4 &v) noexcept {
    // gets the min of this (x, y, z, w) and shuffled (z, w, z, w) and then
    // gets the min of XZ_YW (x', y', z', w') and its shuffled (y', x', w', z')
    const v_i32_4 XZ_YW = min(v, zwzw(v));
    return x(min(XZ_YW, yxwz(XZ_YW)));
  }

  v_i32_4::ValueType min3(const v_i32_4 &v) noexcept {
    // gets the min of (x, y, z, w), (z, w, z, w) and then of (y, x, w, z)
    return x(min(yxwz(v), min(v, zwzw(v))));
  }

  //}
#pragma endregion

#pragma region max()
  //{ max()

  v_i32_4 max(const v_i32_4 &a, const v_i32_4 &b) noexcept {
    return _mm_max_epi32(a.components, b.components);
  }

  v_i32_4::ValueType max(const v_i32_4 &v) noexcept {
    const v_i32_4 XZ_YW = max(v, zwzw(v));
    return x(max(XZ_YW, yxwz(XZ_YW)));
  }

  v_i32_4::ValueType max3(const v_i32_4 &v) noexcept {
    return x(max(yxwz(v), max(v, zwzw(v))));
  }

  //}
#pragma endregion

#pragma region Special vector operations
  //{
  v_i32_4 zeroW(const v_i32_4 &v) noexcept {
    // Shift by four bytes left and then back right to erase the w-component
    return _mm_srli_si128(_mm_slli_si128(v.components, sizeof(v_i32_4::ValueType)), sizeof(v_i32_4::ValueType));
  }

  v_i32_4 andnot(const v_i32_4 &a, const v_i32_4 &b) noexcept {
    return _mm_andnot_si128(a.components, b.components);
  }

  v_i32_4::BoolType isNegative(const v_i32_4 &v) noexcept {
    const v_i32_4::PackedType t = _mm_packs_epi32(v.components, v.components);
    return _mm_movemask_epi8(_mm_packs_epi16(t, t));
  }

  v_i32_4 abs(const v_i32_4 &v) noexcept {
    return _mm_abs_epi32(v.components);
  }

  v_i32_4 clamp(const v_i32_4 &v, const v_i32_4 &l, const v_i32_4 &m) noexcept {
    return max(l, min(v, m));
  }

  inline bool testAllZero(const v_i32_4 &v) noexcept {
    return !!_mm_testz_si128(v.components, MaskAll<v_i32_4::PackedType>());
  }

  inline bool testAllZero3(const v_i32_4 &v) noexcept {
    return !!_mm_testz_si128(v.components, MaskXYZ<v_i32_4>().components);
  }

  inline bool testAllOnes(const v_i32_4 &v) noexcept {
    return !!_mm_testc_si128(v.components, MaskAll<v_i32_4::PackedType>());
  }

  inline bool testAllOnes3(const v_i32_4 &v) noexcept {
    return !!_mm_testc_si128(v.components, MaskXYZ<v_i32_4>().components);
  }

  bool anyTrue(const v_i32_4 &v) noexcept {
    return !testAllZero(v);
  }

  bool anyTrue3(const v_i32_4 &v) noexcept {
    return !testAllZero3(v);
  }

  bool anyFalse(const v_i32_4 &v) noexcept {
    return !testAllOnes(v);
  }

  bool anyFalse3(const v_i32_4 &v) noexcept {
    return !testAllOnes3(v);
  }

  bool allTrue(const v_i32_4 &v) noexcept {
    return testAllOnes(v);
  }

  bool allTrue3(const v_i32_4 &v) noexcept {
    return testAllOnes3(v);
  }

  bool allFalse(const v_i32_4 &v) noexcept {
    return testAllZero(v);
  }

  bool allFalse3(const v_i32_4 &v) noexcept {
    return testAllZero3(v);
  }

  //}
#pragma endregion
}
