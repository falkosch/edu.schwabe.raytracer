#include "vectorization/v_f32_8/type.h"

#include "vectorization/constants/values/zero.h"
#include "vectorization/functions/broadcast.h"

#include <cassert>
#include <cstdint>
#include <cstring>

namespace vectorization {
  v_f32_8::v_f32_8() noexcept
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
      : components(_mm256_setzero_ps())
#else
      : components{_mm_setzero_ps(), _mm_setzero_ps()}
#endif
  {
  }

  v_f32_8::v_f32_8(const PackedType &vector) noexcept : components(vector) {
  }

  v_f32_8::v_f32_8(const ValueType scalar) noexcept
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
      : components(_mm256_set1_ps(scalar))
#else
      : components{_mm_set1_ps(scalar), _mm_set1_ps(scalar)}
#endif
  {
  }

  v_f32_8::v_f32_8(
      const ValueType x1, const ValueType x2, const ValueType x3, const ValueType x4
  ) noexcept
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
      : components(_mm256_set_ps(
            Zero<ValueType>(), Zero<ValueType>(), Zero<ValueType>(), Zero<ValueType>(), x4, x3, x2, x1
        ))
#else
      : components{_mm_set_ps(x4, x3, x2, x1), _mm_setzero_ps()}
#endif
  {
  }

  v_f32_8::v_f32_8(
      const ValueType x1, const ValueType x2, const ValueType x3, const ValueType x4,
      const ValueType x5, const ValueType x6, const ValueType x7, const ValueType x8
  ) noexcept
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
      : components(_mm256_set_ps(x8, x7, x6, x5, x4, x3, x2, x1))
#else
      : components{_mm_set_ps(x4, x3, x2, x1), _mm_set_ps(x8, x7, x6, x5)}
#endif
  {
  }

  v_f32_8::v_f32_8(const ValueType *const values) noexcept
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
      : components(_mm256_load_ps(values))
  {
    assert(reinterpret_cast<std::uintptr_t>(values) % YMM_ALIGNMENT == 0);
  }
#else
      : components{_mm_load_ps(values), _mm_load_ps(values + 4)}
  {
    assert(reinterpret_cast<std::uintptr_t>(values) % XMM_ALIGNMENT == 0);
  }
#endif

  v_f32_8 &v_f32_8::operator=(const PackedType &vector) noexcept {
    components = vector;
    return *this;
  }

  v_f32_8::ValueType &v_f32_8::operator[](const int index) noexcept {
    assert(static_cast<int>(VectorIndices::X1) <= index && static_cast<ASizeT>(index) < SIZE);
    return reinterpret_cast<ValueType *>(this)[index];
  }

  const v_f32_8::ValueType &v_f32_8::operator[](const int index) const noexcept {
    assert(static_cast<int>(VectorIndices::X1) <= index && static_cast<ASizeT>(index) < SIZE);
    return reinterpret_cast<const ValueType *>(this)[index];
  }

  v_f32_8::ValueType &v_f32_8::operator[](const ASizeT index) noexcept {
    assert(index < SIZE);
    return reinterpret_cast<ValueType *>(this)[index];
  }

  const v_f32_8::ValueType &v_f32_8::operator[](const ASizeT index) const noexcept {
    assert(index < SIZE);
    return reinterpret_cast<const ValueType *>(this)[index];
  }

  void store(const v_f32_8 &src, v_f32_8 *const dst) noexcept {
    std::memcpy(&dst->components, &src.components, sizeof(v_f32_8::PackedType));
  }

  void store(const v_f32_8 &src, v_f32_8::PackedType *const dst) noexcept {
    std::memcpy(dst, &src.components, sizeof(v_f32_8::PackedType));
  }

  void store(const v_f32_8 &src, v_f32_8::ValueType *const dst) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    assert(reinterpret_cast<std::uintptr_t>(dst) % YMM_ALIGNMENT == 0);
    _mm256_store_ps(dst, src.components);
#else
    assert(reinterpret_cast<std::uintptr_t>(dst) % XMM_ALIGNMENT == 0);
    _mm_store_ps(dst, src.components.lo);
    _mm_store_ps(dst + 4, src.components.hi);
#endif
  }
}
