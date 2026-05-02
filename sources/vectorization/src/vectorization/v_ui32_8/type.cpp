#include "vectorization/v_ui32_8.h"

#include <cassert>
#include <cstdint>

namespace vectorization {
  v_ui32_8::v_ui32_8() noexcept
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
      : components(Zero<PackedType>())
#else
      : components{Zero<HalfPackedType>(), Zero<HalfPackedType>()}
#endif
  {
  }

  v_ui32_8::v_ui32_8(const PackedType &vector) noexcept : components(vector) {
  }

  v_ui32_8::v_ui32_8(const ValueType scalar) noexcept
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
      : components(_mm256_set1_epi32(static_cast<int>(scalar)))
#else
      : components{_mm_set1_epi32(static_cast<int>(scalar)), _mm_set1_epi32(static_cast<int>(scalar))}
#endif
  {
  }

  v_ui32_8::v_ui32_8(const ValueType x1, const ValueType x2, const ValueType x3, const ValueType x4) noexcept
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
      : components(_mm256_set_epi32(
            0, 0, 0, 0,
            static_cast<int>(x4), static_cast<int>(x3), static_cast<int>(x2), static_cast<int>(x1)
        ))
#else
      : components{_mm_set_epi32(static_cast<int>(x4), static_cast<int>(x3), static_cast<int>(x2), static_cast<int>(x1)),
                    Zero<HalfPackedType>()}
#endif
  {
  }

  v_ui32_8::v_ui32_8(
      const ValueType x1, const ValueType x2, const ValueType x3, const ValueType x4,
      const ValueType x5, const ValueType x6, const ValueType x7, const ValueType x8
  ) noexcept
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
      : components(_mm256_set_epi32(
            static_cast<int>(x8), static_cast<int>(x7), static_cast<int>(x6), static_cast<int>(x5),
            static_cast<int>(x4), static_cast<int>(x3), static_cast<int>(x2), static_cast<int>(x1)
        ))
#else
      : components{_mm_set_epi32(static_cast<int>(x4), static_cast<int>(x3), static_cast<int>(x2), static_cast<int>(x1)),
                    _mm_set_epi32(static_cast<int>(x8), static_cast<int>(x7), static_cast<int>(x6), static_cast<int>(x5))}
#endif
  {
  }

  v_ui32_8::v_ui32_8(const ValueType *const values) noexcept
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

  v_ui32_8 &v_ui32_8::operator=(const PackedType &vector) noexcept {
    components = vector;
    return *this;
  }

  v_ui32_8::ValueType &v_ui32_8::operator[](const int index) noexcept {
    assert(static_cast<int>(VectorIndices::X1) <= index && static_cast<ASizeT>(index) < SIZE);
    return reinterpret_cast<ValueType *>(this)[index];
  }

  const v_ui32_8::ValueType &v_ui32_8::operator[](const int index) const noexcept {
    assert(static_cast<int>(VectorIndices::X1) <= index && static_cast<ASizeT>(index) < SIZE);
    return reinterpret_cast<const ValueType *>(this)[index];
  }

  v_ui32_8::ValueType &v_ui32_8::operator[](const ASizeT index) noexcept {
    assert(index < SIZE);
    return reinterpret_cast<ValueType *>(this)[index];
  }

  const v_ui32_8::ValueType &v_ui32_8::operator[](const ASizeT index) const noexcept {
    assert(index < SIZE);
    return reinterpret_cast<const ValueType *>(this)[index];
  }

  void store(const v_ui32_8 &src, v_ui32_8 *const dst) noexcept {
    dst->components = src.components;
  }

  void store(const v_ui32_8 &src, v_ui32_8::PackedType *const dst) noexcept {
    *dst = src.components;
  }

  void store(const v_ui32_8 &src, v_ui32_8::ValueType *const dst) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    ::_mm256_store_si256(reinterpret_cast<PackedTypes<v_ui32_8::ValueType, v_ui32_8::SIZE>::Type *>(dst), src.components);
#else
    ::_mm_store_si128(reinterpret_cast<v_ui32_8::HalfPackedType *>(dst), src.components.lo);
    ::_mm_store_si128(reinterpret_cast<v_ui32_8::HalfPackedType *>(dst) + 1, src.components.hi);
#endif
  }
}
