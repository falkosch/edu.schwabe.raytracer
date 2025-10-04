#include "vectorization/v_i32_4.h"

#include <cassert>
#include <limits>

namespace vectorization {
  v_i32_4::v_i32_4() noexcept : components(Zero<PackedType>()) {
  }

  v_i32_4::v_i32_4(const PackedType &v) noexcept : components(v) {
  }

  v_i32_4::v_i32_4(const ValueType s) noexcept : components(_mm_set1_epi32(s)) {
  }

  v_i32_4::v_i32_4(const ValueType x, const ValueType y) noexcept
      : components(_mm_set_epi32(Zero<ValueType>(), Zero<ValueType>(), y, x)) {
  }

  v_i32_4::v_i32_4(const ValueType x, const ValueType y, const ValueType z) noexcept
      : components(_mm_set_epi32(Zero<ValueType>(), z, y, x)) {
  }

  v_i32_4::v_i32_4(const ValueType x, const ValueType y, const ValueType z, const ValueType w) noexcept
      : components(_mm_set_epi32(w, z, y, x)) {
  }

  v_i32_4 &v_i32_4::operator=(const PackedType &v) noexcept {
    components = v;
    return *this;
  }

  v_i32_4::ValueType &v_i32_4::operator[](const int index) noexcept {
    assert(static_cast<int>(VectorIndices::X) <= index && static_cast<ASizeT>(index) < SIZE);
    return reinterpret_cast<ValueType *const>(this)[index];
  }

  const v_i32_4::ValueType &v_i32_4::operator[](const int index) const noexcept {
    assert(static_cast<int>(VectorIndices::X) <= index && static_cast<ASizeT>(index) < SIZE);
    return reinterpret_cast<const ValueType *const>(this)[index];
  }

  v_i32_4::ValueType &v_i32_4::operator[](const ASizeT index) noexcept {
    assert(index < SIZE);
    return reinterpret_cast<ValueType *const>(this)[index];
  }

  const v_i32_4::ValueType &v_i32_4::operator[](const ASizeT index) const noexcept {
    assert(index < SIZE);
    return reinterpret_cast<const ValueType *const>(this)[index];
  }
}
