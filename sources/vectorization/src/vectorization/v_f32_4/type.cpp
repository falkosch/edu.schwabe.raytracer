#include "vectorization/v_f32_4/type.h"

#include "vectorization/constants.h"

#include <array>
#include <cassert>

namespace vectorization {
  v_f32_4::v_f32_4() noexcept : components(Zero<PackedType>()) {
  }

  v_f32_4::v_f32_4(const PackedType &v) noexcept : components(v) {
  }

  v_f32_4::v_f32_4(const ValueType v) noexcept : components(_mm_set_ps1(v)) {
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

  v_f32_4::v_f32_4(const PackedType *const v) noexcept : components() {
    std::memcpy(&this->components, v, sizeof(PackedType));
  }

  v_f32_4::v_f32_4(const VectorType *const v) noexcept : components() {
    std::memcpy(&this->components, &v->components, sizeof(PackedType));
  }

  v_f32_4::v_f32_4(const ValueType *const v) noexcept : components(_mm_load_ps(v)) {
  }

  v_f32_4 &v_f32_4::operator=(const PackedType &v) noexcept {
    components = v;
    return *this;
  }

  const v_f32_4::ValueType &v_f32_4::operator[](const ASizeT index) const noexcept {
    assert(index < SIZE);
    return reinterpret_cast<const ValueType *>(&this->components)[index];
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
