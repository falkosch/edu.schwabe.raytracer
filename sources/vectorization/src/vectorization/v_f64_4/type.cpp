#include "vectorization/v_f64_4/type.h"

#include "vectorization/constants/values/zero.h"
#include "vectorization/functions/broadcast.h"

#include <cassert>
#include <cstdint>
#include <cstring>

namespace vectorization {
  v_f64_4::v_f64_4() noexcept
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
      : components(_mm256_setzero_pd())
#else
      : components{_mm_setzero_pd(), _mm_setzero_pd()}
#endif
  {
  }

  v_f64_4::v_f64_4(const PackedType &vector) noexcept : components(vector) {
  }

  v_f64_4::v_f64_4(const ValueType scalar) noexcept
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
      : components(_mm256_set1_pd(scalar))
#else
      : components{_mm_set1_pd(scalar), _mm_set1_pd(scalar)}
#endif
  {
  }

  v_f64_4::v_f64_4(
      const ValueType x, const ValueType y, const ValueType z, const ValueType w
  ) noexcept
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
      : components(_mm256_set_pd(w, z, y, x))
#else
      : components{_mm_set_pd(y, x), _mm_set_pd(w, z)}
#endif
  {
  }

  v_f64_4::v_f64_4(const ValueType *const values) noexcept
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
      : components(_mm256_load_pd(values))
  {
    assert(reinterpret_cast<std::uintptr_t>(values) % YMM_ALIGNMENT == 0);
  }
#else
      : components{_mm_load_pd(values), _mm_load_pd(values + 2)}
  {
    assert(reinterpret_cast<std::uintptr_t>(values) % XMM_ALIGNMENT == 0);
  }
#endif

  v_f64_4 &v_f64_4::operator=(const PackedType &vector) noexcept {
    components = vector;
    return *this;
  }

  v_f64_4::ValueType &v_f64_4::operator[](const int index) noexcept {
    assert(static_cast<int>(VectorIndices::X) <= index && static_cast<ASizeT>(index) < SIZE);
    return reinterpret_cast<ValueType *>(this)[index];
  }

  const v_f64_4::ValueType &v_f64_4::operator[](const int index) const noexcept {
    assert(static_cast<int>(VectorIndices::X) <= index && static_cast<ASizeT>(index) < SIZE);
    return reinterpret_cast<const ValueType *>(this)[index];
  }

  v_f64_4::ValueType &v_f64_4::operator[](const ASizeT index) noexcept {
    assert(index < SIZE);
    return reinterpret_cast<ValueType *>(this)[index];
  }

  const v_f64_4::ValueType &v_f64_4::operator[](const ASizeT index) const noexcept {
    assert(index < SIZE);
    return reinterpret_cast<const ValueType *>(this)[index];
  }

  void store(const v_f64_4 &src, v_f64_4 *const dst) noexcept {
    std::memcpy(&dst->components, &src.components, sizeof(v_f64_4::PackedType));
  }

  void store(const v_f64_4 &src, v_f64_4::PackedType *const dst) noexcept {
    std::memcpy(dst, &src.components, sizeof(v_f64_4::PackedType));
  }

  void store(const v_f64_4 &src, v_f64_4::ValueType *const dst) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    assert(reinterpret_cast<std::uintptr_t>(dst) % YMM_ALIGNMENT == 0);
    _mm256_store_pd(dst, src.components);
#else
    assert(reinterpret_cast<std::uintptr_t>(dst) % XMM_ALIGNMENT == 0);
    _mm_store_pd(dst, src.components.lo);
    _mm_store_pd(dst + 2, src.components.hi);
#endif
  }
}
