#include "vectorization/v_f64_2/type.h"

#include "vectorization/constants/values.h"
#include "vectorization/functions/broadcast.h"

#include <array>
#include <cassert>
#include <cstdint>

namespace vectorization {
  v_f64_2::v_f64_2() noexcept : components(Zero<PackedType>()) {
  }

  v_f64_2::v_f64_2(const PackedType &vector) noexcept : components(vector) {
  }

  v_f64_2::v_f64_2(const ValueType scalar) noexcept : components(broadcast(scalar)) {
  }

  v_f64_2::v_f64_2(const ValueType x, const ValueType y) noexcept : components(_mm_set_pd(y, x)) {
  }

  v_f64_2::v_f64_2(const PackedType *const vector) noexcept : components() {
    std::memcpy(&this->components, vector, sizeof(PackedType));
  }

  v_f64_2::v_f64_2(const VectorType *const vector) noexcept : components() {
    std::memcpy(&this->components, &vector->components, sizeof(PackedType));
  }

  v_f64_2::v_f64_2(const ValueType *const values) noexcept : components(_mm_load_pd(values)) {
    assert(reinterpret_cast<std::uintptr_t>(values) % XMM_ALIGNMENT == 0);
  }

  v_f64_2 &v_f64_2::operator=(const PackedType &vector) noexcept {
    components = vector;
    return *this;
  }

  v_f64_2::ValueType &v_f64_2::operator[](const int index) noexcept {
    assert(static_cast<int>(VectorIndices::X) <= index && static_cast<ASizeT>(index) < SIZE);
    return reinterpret_cast<ValueType *const>(this)[index];
  }

  const v_f64_2::ValueType &v_f64_2::operator[](const int index) const noexcept {
    assert(static_cast<int>(VectorIndices::X) <= index && static_cast<ASizeT>(index) < SIZE);
    return reinterpret_cast<const ValueType *const>(this)[index];
  }

  v_f64_2::ValueType &v_f64_2::operator[](const ASizeT index) noexcept {
    assert(index < SIZE);
    return reinterpret_cast<ValueType *const>(this)[index];
  }

  const v_f64_2::ValueType &v_f64_2::operator[](const ASizeT index) const noexcept {
    assert(index < SIZE);
    return reinterpret_cast<const ValueType *const>(this)[index];
  }

  void store(const v_f64_2 &src, v_f64_2 *const dst) noexcept {
    std::memcpy(&dst->components, &src.components, sizeof(v_f64_2::PackedType));
  }

  void store(const v_f64_2 &src, v_f64_2::PackedType *const dst) noexcept {
    std::memcpy(dst, &src.components, sizeof(v_f64_2::PackedType));
  }

  void store(const v_f64_2 &src, v_f64_2::ValueType *const dst) noexcept {
    assert(reinterpret_cast<std::uintptr_t>(dst) % XMM_ALIGNMENT == 0);
    _mm_store_pd(dst, src.components);
  }
}
