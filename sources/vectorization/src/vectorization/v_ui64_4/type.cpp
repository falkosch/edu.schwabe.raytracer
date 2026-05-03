#include "vectorization/v_ui64_4.h"

#include <cassert>
#include <cstdint>
#include <cstring>

namespace vectorization {
  v_ui64_4::v_ui64_4() noexcept
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
      : components(Zero<PackedType>())
#else
      : components{Zero<HalfPackedType>(), Zero<HalfPackedType>()}
#endif
  {
  }

  v_ui64_4::v_ui64_4(const PackedType &vector) noexcept : components(vector) {
  }

  v_ui64_4::v_ui64_4(const ValueType scalar) noexcept
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
      : components(_mm256_set1_epi64x(static_cast<long long>(scalar)))
#else
      : components{_mm_set1_epi64x(static_cast<long long>(scalar)), _mm_set1_epi64x(static_cast<long long>(scalar))}
#endif
  {
  }

  v_ui64_4::v_ui64_4(const ValueType x, const ValueType y, const ValueType z, const ValueType w) noexcept
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
      : components(_mm256_set_epi64x(
            static_cast<long long>(w), static_cast<long long>(z),
            static_cast<long long>(y), static_cast<long long>(x)))
#else
      : components{_mm_set_epi64x(static_cast<long long>(y), static_cast<long long>(x)),
                   _mm_set_epi64x(static_cast<long long>(w), static_cast<long long>(z))}
#endif
  {
  }

  v_ui64_4::v_ui64_4(const ValueType *const values) noexcept
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
      : components(::_mm256_load_si256(reinterpret_cast<const PackedTypes<ValueType, SIZE>::Type *>(values)))
  {
    assert(reinterpret_cast<std::uintptr_t>(values) % YMM_ALIGNMENT == 0);
  }
#else
      : components{
            ::_mm_load_si128(reinterpret_cast<const HalfPackedType *>(values)),
            ::_mm_load_si128(reinterpret_cast<const HalfPackedType *>(values) + 1)}
  {
    assert(reinterpret_cast<std::uintptr_t>(values) % XMM_ALIGNMENT == 0);
  }
#endif

  v_ui64_4 &v_ui64_4::operator=(const PackedType &vector) noexcept {
    components = vector;
    return *this;
  }

  v_ui64_4::ValueType &v_ui64_4::operator[](const int index) noexcept {
    assert(static_cast<int>(VectorIndices::X) <= index && static_cast<ASizeT>(index) < SIZE);
    return reinterpret_cast<ValueType *>(this)[index];
  }

  const v_ui64_4::ValueType &v_ui64_4::operator[](const int index) const noexcept {
    assert(static_cast<int>(VectorIndices::X) <= index && static_cast<ASizeT>(index) < SIZE);
    return reinterpret_cast<const ValueType *>(this)[index];
  }

  v_ui64_4::ValueType &v_ui64_4::operator[](const ASizeT index) noexcept {
    assert(index < SIZE);
    return reinterpret_cast<ValueType *>(this)[index];
  }

  const v_ui64_4::ValueType &v_ui64_4::operator[](const ASizeT index) const noexcept {
    assert(index < SIZE);
    return reinterpret_cast<const ValueType *>(this)[index];
  }

  void store(const v_ui64_4 &src, v_ui64_4 *const dst) noexcept {
    dst->components = src.components;
  }

  void store(const v_ui64_4 &src, v_ui64_4::PackedType *const dst) noexcept {
    *dst = src.components;
  }

  void store(const v_ui64_4 &src, v_ui64_4::ValueType *const dst) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    ::_mm256_store_si256(reinterpret_cast<PackedTypes<v_ui64_4::ValueType, v_ui64_4::SIZE>::Type *>(dst), src.components);
#else
    ::_mm_store_si128(reinterpret_cast<v_ui64_4::HalfPackedType *>(dst), src.components.lo);
    ::_mm_store_si128(reinterpret_cast<v_ui64_4::HalfPackedType *>(dst) + 1, src.components.hi);
#endif
  }
}
