#include "vectorization/v_ui32_4.h"

#include <cassert>

namespace vectorization {
  v_ui32_4::v_ui32_4() noexcept : components(Zero<PackedType>()) {
  }

  v_ui32_4::v_ui32_4(const PackedType &vector) noexcept : components(vector) {
  }

  v_ui32_4::v_ui32_4(const ValueType scalar) noexcept : components(_mm_set1_epi32(static_cast<int>(scalar))) {
  }

  v_ui32_4::v_ui32_4(const ValueType x, const ValueType y) noexcept
      : components(_mm_set_epi32(Zero<int>(), Zero<int>(), static_cast<int>(y), static_cast<int>(x))) {
  }

  v_ui32_4::v_ui32_4(const ValueType x, const ValueType y, const ValueType z) noexcept
      : components(_mm_set_epi32(Zero<int>(), static_cast<int>(z), static_cast<int>(y), static_cast<int>(x))) {
  }

  v_ui32_4::v_ui32_4(
      const ValueType x, const ValueType y, const ValueType z, const ValueType w
  ) noexcept
      : components(_mm_set_epi32(static_cast<int>(w), static_cast<int>(z), static_cast<int>(y), static_cast<int>(x))) {
  }

  v_ui32_4 &v_ui32_4::operator=(const PackedType &vector) noexcept {
    components = vector;
    return *this;
  }

  v_ui32_4::ValueType &v_ui32_4::operator[](const int index) noexcept {
    assert(static_cast<int>(VectorIndices::X) <= index && static_cast<ASizeT>(index) < SIZE);
    return reinterpret_cast<ValueType *const>(this)[index];
  }

  const v_ui32_4::ValueType &v_ui32_4::operator[](const int index) const noexcept {
    assert(static_cast<int>(VectorIndices::X) <= index && static_cast<ASizeT>(index) < SIZE);
    return reinterpret_cast<const ValueType *const>(this)[index];
  }

  v_ui32_4::ValueType &v_ui32_4::operator[](const ASizeT index) noexcept {
    assert(index < SIZE);
    return reinterpret_cast<ValueType *const>(this)[index];
  }

  const v_ui32_4::ValueType &v_ui32_4::operator[](const ASizeT index) const noexcept {
    assert(index < SIZE);
    return reinterpret_cast<const ValueType *const>(this)[index];
  }
}
