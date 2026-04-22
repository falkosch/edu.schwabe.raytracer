#include "vectorization/v_f32_4/type.h"

#include "vectorization/constants/values.h"
#include "vectorization/functions/broadcast.h"

#include <array>
#include <cassert>

namespace vectorization {
  v_f32_4::v_f32_4() noexcept : components(Zero<PackedType>()) {
  }

  v_f32_4::v_f32_4(const PackedType &vector) noexcept : components(vector) {
  }

  v_f32_4::v_f32_4(const ValueType scalar) noexcept : components(broadcast(scalar)) {
  }

  v_f32_4::v_f32_4(const ValueType x, const ValueType y) noexcept
      : components(_mm_set_ps(Zero<ValueType>(), Zero<ValueType>(), y, x)) {
  }

  v_f32_4::v_f32_4(const ValueType x, const ValueType y, const ValueType z) noexcept
      : components(_mm_set_ps(Zero<ValueType>(), z, y, x)) {
  }

  v_f32_4::v_f32_4(const ValueType x, const ValueType y, const ValueType z, const ValueType w) noexcept
      : components(_mm_set_ps(w, z, y, x)) {
  }

  v_f32_4::v_f32_4(const PackedType *const vector) noexcept : components() {
    std::memcpy(&this->components, vector, sizeof(PackedType));
  }

  v_f32_4::v_f32_4(const VectorType *const vector) noexcept : components() {
    std::memcpy(&this->components, &vector->components, sizeof(PackedType));
  }

  v_f32_4::v_f32_4(const ValueType *const values) noexcept : components(_mm_load_ps(values)) {
  }

  v_f32_4 &v_f32_4::operator=(const PackedType &vector) noexcept {
    components = vector;
    return *this;
  }

  v_f32_4::ValueType &v_f32_4::operator[](const int index) noexcept {
    assert(static_cast<int>(VectorIndices::X) <= index && static_cast<ASizeT>(index) < SIZE);
    return reinterpret_cast<ValueType *const>(this)[index];
  }

  const v_f32_4::ValueType &v_f32_4::operator[](const int index) const noexcept {
    assert(static_cast<int>(VectorIndices::X) <= index && static_cast<ASizeT>(index) < SIZE);
    return reinterpret_cast<const ValueType *const>(this)[index];
  }

  v_f32_4::ValueType &v_f32_4::operator[](const ASizeT index) noexcept {
    assert(index < SIZE);
    return reinterpret_cast<ValueType *const>(this)[index];
  }

  const v_f32_4::ValueType &v_f32_4::operator[](const ASizeT index) const noexcept {
    assert(index < SIZE);
    return reinterpret_cast<const ValueType *const>(this)[index];
  }

  void store(const v_f32_4 &src, v_f32_4 *const dst) noexcept {
    std::memcpy(&dst->components, &src.components, sizeof(v_f32_4::PackedType));
  }

  void store(const v_f32_4 &src, v_f32_4::PackedType *const dst) noexcept {
    std::memcpy(dst, &src.components, sizeof(v_f32_4::PackedType));
  }

  void store(const v_f32_4 &src, v_f32_4::ValueType *const dst) noexcept {
    _mm_store_ps(dst, src.components);
  }
}
