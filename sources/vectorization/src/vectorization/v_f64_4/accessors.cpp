#include "vectorization/v_f64_4/accessors.h"

#include "vectorization/accessors/component_256d.h"
#include "vectorization/accessors/component_128d.h"
#include "vectorization/accessors/replace_component_256d.h"
#include "vectorization/accessors/replace_component_128d.h"

namespace vectorization {
  template <>
  v_f64_4::ValueType component<VectorIndices::X>(const v_f64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return component<VectorIndices::X>(v.components);
#else
    return component<VectorIndices::X>(v.components.lo);
#endif
  }

  template <>
  v_f64_4::ValueType component<VectorIndices::Y>(const v_f64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return component<VectorIndices::Y>(v.components);
#else
    return component<VectorIndices::Y>(v.components.lo);
#endif
  }

  template <>
  v_f64_4::ValueType component<VectorIndices::Z>(const v_f64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return component<VectorIndices::Z>(v.components);
#else
    return component<VectorIndices::X>(v.components.hi);
#endif
  }

  template <>
  v_f64_4::ValueType component<VectorIndices::W>(const v_f64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return component<VectorIndices::W>(v.components);
#else
    return component<VectorIndices::Y>(v.components.hi);
#endif
  }

  v_f64_4::ValueType x(const v_f64_4 &v) noexcept { return component<VectorIndices::X>(v); }
  v_f64_4::ValueType y(const v_f64_4 &v) noexcept { return component<VectorIndices::Y>(v); }
  v_f64_4::ValueType z(const v_f64_4 &v) noexcept { return component<VectorIndices::Z>(v); }
  v_f64_4::ValueType w(const v_f64_4 &v) noexcept { return component<VectorIndices::W>(v); }

  template <>
  v_f64_4 replaceComponent<VectorIndices::X>(const v_f64_4 &v, const v_f64_4::ValueType s) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return replaceComponent<VectorIndices::X>(v.components, s);
#else
    return {{replaceComponent<VectorIndices::X>(v.components.lo, s), v.components.hi}};
#endif
  }

  template <>
  v_f64_4 replaceComponent<VectorIndices::Y>(const v_f64_4 &v, const v_f64_4::ValueType s) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return replaceComponent<VectorIndices::Y>(v.components, s);
#else
    return {{replaceComponent<VectorIndices::Y>(v.components.lo, s), v.components.hi}};
#endif
  }

  template <>
  v_f64_4 replaceComponent<VectorIndices::Z>(const v_f64_4 &v, const v_f64_4::ValueType s) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return replaceComponent<VectorIndices::Z>(v.components, s);
#else
    return {{v.components.lo, replaceComponent<VectorIndices::X>(v.components.hi, s)}};
#endif
  }

  template <>
  v_f64_4 replaceComponent<VectorIndices::W>(const v_f64_4 &v, const v_f64_4::ValueType s) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return replaceComponent<VectorIndices::W>(v.components, s);
#else
    return {{v.components.lo, replaceComponent<VectorIndices::Y>(v.components.hi, s)}};
#endif
  }

  v_f64_4 replaceX(const v_f64_4 &v, const v_f64_4::ValueType s) noexcept { return replaceComponent<VectorIndices::X>(v, s); }
  v_f64_4 replaceY(const v_f64_4 &v, const v_f64_4::ValueType s) noexcept { return replaceComponent<VectorIndices::Y>(v, s); }
  v_f64_4 replaceZ(const v_f64_4 &v, const v_f64_4::ValueType s) noexcept { return replaceComponent<VectorIndices::Z>(v, s); }
  v_f64_4 replaceW(const v_f64_4 &v, const v_f64_4::ValueType s) noexcept { return replaceComponent<VectorIndices::W>(v, s); }
}
