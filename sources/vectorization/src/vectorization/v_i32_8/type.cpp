#include "vectorization/v_i32_8.h"

#include <cassert>
#include <cstdint>
#include <cstring>

namespace vectorization {
  v_i32_8::v_i32_8() noexcept
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
      : components(Zero<PackedType>())
#else
      : components{Zero<HalfPackedType>(), Zero<HalfPackedType>()}
#endif
  {
  }

  v_i32_8::v_i32_8(const PackedType &vector) noexcept : components(vector) {
  }

  v_i32_8::v_i32_8(const ValueType scalar) noexcept
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
      : components(_mm256_set1_epi32(scalar))
#else
      : components{_mm_set1_epi32(scalar), _mm_set1_epi32(scalar)}
#endif
  {
  }

  v_i32_8::v_i32_8(const ValueType x1, const ValueType x2, const ValueType x3, const ValueType x4) noexcept
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
      : components(_mm256_set_epi32(
            Zero<ValueType>(), Zero<ValueType>(), Zero<ValueType>(), Zero<ValueType>(), x4, x3, x2, x1
        ))
#else
      : components{_mm_set_epi32(x4, x3, x2, x1), Zero<HalfPackedType>()}
#endif
  {
  }

  v_i32_8::v_i32_8(
      const ValueType x1, const ValueType x2, const ValueType x3, const ValueType x4,
      const ValueType x5, const ValueType x6, const ValueType x7, const ValueType x8
  ) noexcept
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
      : components(_mm256_set_epi32(x8, x7, x6, x5, x4, x3, x2, x1))
#else
      : components{_mm_set_epi32(x4, x3, x2, x1), _mm_set_epi32(x8, x7, x6, x5)}
#endif
  {
  }

  v_i32_8::v_i32_8(const ValueType *const values) noexcept
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

  v_i32_8 &v_i32_8::operator=(const PackedType &vector) noexcept {
    components = vector;
    return *this;
  }

  v_i32_8::ValueType &v_i32_8::operator[](const int index) noexcept {
    assert(static_cast<int>(VectorIndices::X1) <= index && static_cast<ASizeT>(index) < SIZE);
    return reinterpret_cast<ValueType *>(this)[index];
  }

  const v_i32_8::ValueType &v_i32_8::operator[](const int index) const noexcept {
    assert(static_cast<int>(VectorIndices::X1) <= index && static_cast<ASizeT>(index) < SIZE);
    return reinterpret_cast<const ValueType *>(this)[index];
  }

  v_i32_8::ValueType &v_i32_8::operator[](const ASizeT index) noexcept {
    assert(index < SIZE);
    return reinterpret_cast<ValueType *>(this)[index];
  }

  const v_i32_8::ValueType &v_i32_8::operator[](const ASizeT index) const noexcept {
    assert(index < SIZE);
    return reinterpret_cast<const ValueType *>(this)[index];
  }

  void store(const v_i32_8 &src, v_i32_8 *const dst) noexcept {
    dst->components = src.components;
  }

  void store(const v_i32_8 &src, v_i32_8::PackedType *const dst) noexcept {
    *dst = src.components;
  }

  void store(const v_i32_8 &src, v_i32_8::ValueType *const dst) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    ::_mm256_store_si256(reinterpret_cast<PackedTypes<v_i32_8::ValueType, v_i32_8::SIZE>::Type *>(dst), src.components);
#else
    ::_mm_store_si128(reinterpret_cast<v_i32_8::HalfPackedType *>(dst), src.components.lo);
    ::_mm_store_si128(reinterpret_cast<v_i32_8::HalfPackedType *>(dst) + 1, src.components.hi);
#endif
  }
}
