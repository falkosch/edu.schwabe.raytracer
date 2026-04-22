#include "vectorization/v_i32_4.h"

#include <cassert>
#include <cstdint>
#include <cstring>
#include <limits>

namespace vectorization {
  v_i32_4::v_i32_4() noexcept : components(Zero<PackedType>()) {
  }

  v_i32_4::v_i32_4(const PackedType &vector) noexcept : components(vector) {
  }

  v_i32_4::v_i32_4(const ValueType scalar) noexcept : components(_mm_set1_epi32(scalar)) {
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

  v_i32_4::v_i32_4(const PackedType *const vector) noexcept : components() {
    std::memcpy(&this->components, vector, sizeof(PackedType));
  }

  v_i32_4::v_i32_4(const VectorType *const vector) noexcept : components() {
    std::memcpy(&this->components, &vector->components, sizeof(PackedType));
  }

  v_i32_4::v_i32_4(const ValueType *const values) noexcept
      : components(_mm_load_si128(reinterpret_cast<const PackedType *>(values))) {
    assert(reinterpret_cast<std::uintptr_t>(values) % XMM_ALIGNMENT == 0);
  }

  v_i32_4 &v_i32_4::operator=(const PackedType &vector) noexcept {
    components = vector;
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
