#include "vectorization/v_ui64_2.h"

#ifndef ARCH_X64
#include <array>
#endif

#include <cassert>

namespace vectorization {
  v_ui64_2::v_ui64_2() noexcept : components(Zero<PackedType>()) {
  }

  v_ui64_2::v_ui64_2(const PackedType &vector) noexcept : components(vector) {
  }

#ifdef ARCH_X64
  v_ui64_2::v_ui64_2(const ValueType scalar) noexcept : components(_mm_set1_epi64x(static_cast<long long>(scalar))) {
  }

  v_ui64_2::v_ui64_2(const ValueType x, const ValueType y) noexcept
      : components(_mm_set_epi64x(static_cast<long long>(y), static_cast<long long>(x))) {
  }
#else
  v_ui64_2::v_ui64_2(const v_ui64_2::ValueType scalar) noexcept : components() {
    std::array<v_ui64_2::ValueType, v_ui64_2::SIZE> broadcast = {scalar, scalar};
    std::memcpy(&(this->components), broadcast.data(), sizeof(v_ui64_2::PackedType));
  }

  v_ui64_2::v_ui64_2(const v_ui64_2::ValueType x, const v_ui64_2::ValueType y) noexcept : components() {
    std::array<v_ui64_2::ValueType, v_ui64_2::SIZE> broadcast = {x, y};
    std::memcpy(&(this->components), broadcast.data(), sizeof(v_ui64_2::PackedType));
  }
#endif

  v_ui64_2 &v_ui64_2::operator=(const PackedType &packed) noexcept {
    components = packed;
    return *this;
  }

  v_ui64_2::ValueType &v_ui64_2::operator[](const int index) noexcept {
    assert(static_cast<int>(VectorIndices::X) <= index && static_cast<ASizeT>(index) < SIZE);
    return reinterpret_cast<ValueType *const>(this)[index];
  }

  const v_ui64_2::ValueType &v_ui64_2::operator[](const int index) const noexcept {
    assert(static_cast<int>(VectorIndices::X) <= index && static_cast<ASizeT>(index) < SIZE);
    return reinterpret_cast<const ValueType *const>(this)[index];
  }

  v_ui64_2::ValueType &v_ui64_2::operator[](const ASizeT index) noexcept {
    assert(index < SIZE);
    return reinterpret_cast<ValueType *const>(this)[index];
  }

  const v_ui64_2::ValueType &v_ui64_2::operator[](const ASizeT index) const noexcept {
    assert(index < SIZE);
    return reinterpret_cast<const ValueType *const>(this)[index];
  }
}
