#include "vectorization/v_ui64_2.h"

#ifndef ARCH_X64
#include <array>
#endif

namespace vectorization {
  template <>
  v_ui64_2::ValueType component<VectorIndices::X>(const v_ui64_2 &vector) noexcept {
#ifdef ARCH_X64
    return static_cast<v_ui64_2::ValueType>(_mm_cvtsi128_si64x(vector.components));
#else
    std::array<v_ui64_2::ValueType, v_ui64_2::SIZE> components;
    std::memcpy(components.data(), &vector.components, sizeof(v_ui64_2::PackedType));
    return components.at(VectorIndices::X);
#endif
  }

  template <>
  v_ui64_2::ValueType component<VectorIndices::Y>(const v_ui64_2 &vector) noexcept {
#ifdef ARCH_X64
    return static_cast<v_ui64_2::ValueType>(_mm_cvtsi128_si64x(_mm_unpackhi_epi64(vector.components, vector.components))
    );
#else
    std::array<v_ui64_2::ValueType, v_ui64_2::SIZE> components;
    std::memcpy(components.data(), &vector.components, sizeof(v_ui64_2::PackedType));
    return components.at(VectorIndices::Y);
#endif
  }

  v_ui64_2::ValueType x(const v_ui64_2 &vector) noexcept {
    return component<VectorIndices::X>(vector);
  }

  v_ui64_2::ValueType y(const v_ui64_2 &vector) noexcept {
    return component<VectorIndices::Y>(vector);
  }

  template <>
  v_ui64_2 replaceComponent<VectorIndices::X>(const v_ui64_2 &vector, const v_ui64_2::ValueType scalar) noexcept {
#ifdef ARCH_X64
    return _mm_insert_epi64(vector.components, static_cast<long long>(scalar), VectorIndices::X);
#else
    return v_ui64_2(scalar, y(vector));
#endif
  }

  template <>
  v_ui64_2 replaceComponent<VectorIndices::Y>(const v_ui64_2 &vector, const v_ui64_2::ValueType scalar) noexcept {
#ifdef ARCH_X64
    return _mm_insert_epi64(vector.components, static_cast<long long>(scalar), VectorIndices::Y);
#else
    return v_ui64_2(x(vector), scalar);
#endif
  }

  v_ui64_2 replaceX(const v_ui64_2 &vector, const v_ui64_2::ValueType scalar) noexcept {
    return replaceComponent<VectorIndices::X>(vector, scalar);
  }

  v_ui64_2 replaceY(const v_ui64_2 &vector, const v_ui64_2::ValueType scalar) noexcept {
    return replaceComponent<VectorIndices::Y>(vector, scalar);
  }
}
