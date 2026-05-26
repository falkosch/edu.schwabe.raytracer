#include "vectorization/v_ui32_8/accessors.h"

#include "vectorization/accessors/component_128i.h"
#include "vectorization/accessors/replace_component_128i.h"
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
#include "vectorization/accessors/component_256i.h"
#include "vectorization/accessors/replace_component_256i.h"
#endif

namespace vectorization
{
  template <>
  v_ui32_8::ValueType component<VectorIndices::X1>(const v_ui32_8& v) noexcept
  {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return static_cast<v_ui32_8::ValueType>(component<VectorIndices::X1>(v.components));
#else
    return static_cast<v_ui32_8::ValueType>(component<VectorIndices::X>(v.components.lo));
#endif
  }

  template <>
  v_ui32_8::ValueType component<VectorIndices::X2>(const v_ui32_8& v) noexcept
  {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return static_cast<v_ui32_8::ValueType>(component<VectorIndices::X2>(v.components));
#else
    return static_cast<v_ui32_8::ValueType>(component<VectorIndices::Y>(v.components.lo));
#endif
  }

  template <>
  v_ui32_8::ValueType component<VectorIndices::X3>(const v_ui32_8& v) noexcept
  {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return static_cast<v_ui32_8::ValueType>(component<VectorIndices::X3>(v.components));
#else
    return static_cast<v_ui32_8::ValueType>(component<VectorIndices::Z>(v.components.lo));
#endif
  }

  template <>
  v_ui32_8::ValueType component<VectorIndices::X4>(const v_ui32_8& v) noexcept
  {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return static_cast<v_ui32_8::ValueType>(component<VectorIndices::X4>(v.components));
#else
    return static_cast<v_ui32_8::ValueType>(component<VectorIndices::W>(v.components.lo));
#endif
  }

  template <>
  v_ui32_8::ValueType component<VectorIndices::X5>(const v_ui32_8& v) noexcept
  {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return static_cast<v_ui32_8::ValueType>(component<VectorIndices::X5>(v.components));
#else
    return static_cast<v_ui32_8::ValueType>(component<VectorIndices::X>(v.components.hi));
#endif
  }

  template <>
  v_ui32_8::ValueType component<VectorIndices::X6>(const v_ui32_8& v) noexcept
  {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return static_cast<v_ui32_8::ValueType>(component<VectorIndices::X6>(v.components));
#else
    return static_cast<v_ui32_8::ValueType>(component<VectorIndices::Y>(v.components.hi));
#endif
  }

  template <>
  v_ui32_8::ValueType component<VectorIndices::X7>(const v_ui32_8& v) noexcept
  {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return static_cast<v_ui32_8::ValueType>(component<VectorIndices::X7>(v.components));
#else
    return static_cast<v_ui32_8::ValueType>(component<VectorIndices::Z>(v.components.hi));
#endif
  }

  template <>
  v_ui32_8::ValueType component<VectorIndices::X8>(const v_ui32_8& v) noexcept
  {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return static_cast<v_ui32_8::ValueType>(component<VectorIndices::X8>(v.components));
#else
    return static_cast<v_ui32_8::ValueType>(component<VectorIndices::W>(v.components.hi));
#endif
  }

  v_ui32_8::ValueType x1(const v_ui32_8& v) noexcept { return component<VectorIndices::X1>(v); }
  v_ui32_8::ValueType x2(const v_ui32_8& v) noexcept { return component<VectorIndices::X2>(v); }
  v_ui32_8::ValueType x3(const v_ui32_8& v) noexcept { return component<VectorIndices::X3>(v); }
  v_ui32_8::ValueType x4(const v_ui32_8& v) noexcept { return component<VectorIndices::X4>(v); }
  v_ui32_8::ValueType x5(const v_ui32_8& v) noexcept { return component<VectorIndices::X5>(v); }
  v_ui32_8::ValueType x6(const v_ui32_8& v) noexcept { return component<VectorIndices::X6>(v); }
  v_ui32_8::ValueType x7(const v_ui32_8& v) noexcept { return component<VectorIndices::X7>(v); }
  v_ui32_8::ValueType x8(const v_ui32_8& v) noexcept { return component<VectorIndices::X8>(v); }

  template <>
  v_ui32_8 replaceComponent<VectorIndices::X1>(const v_ui32_8& v, const v_ui32_8::ValueType s) noexcept
  {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return replaceComponent<VectorIndices::X1>(v.components, static_cast<Int_32>(s));
#else
    return {{replaceComponent<VectorIndices::X>(v.components.lo, static_cast<Int_32>(s)), v.components.hi}};
#endif
  }

  template <>
  v_ui32_8 replaceComponent<VectorIndices::X2>(const v_ui32_8& v, const v_ui32_8::ValueType s) noexcept
  {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return replaceComponent<VectorIndices::X2>(v.components, static_cast<Int_32>(s));
#else
    return {{replaceComponent<VectorIndices::Y>(v.components.lo, static_cast<Int_32>(s)), v.components.hi}};
#endif
  }

  template <>
  v_ui32_8 replaceComponent<VectorIndices::X3>(const v_ui32_8& v, const v_ui32_8::ValueType s) noexcept
  {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return replaceComponent<VectorIndices::X3>(v.components, static_cast<Int_32>(s));
#else
    return {{replaceComponent<VectorIndices::Z>(v.components.lo, static_cast<Int_32>(s)), v.components.hi}};
#endif
  }

  template <>
  v_ui32_8 replaceComponent<VectorIndices::X4>(const v_ui32_8& v, const v_ui32_8::ValueType s) noexcept
  {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return replaceComponent<VectorIndices::X4>(v.components, static_cast<Int_32>(s));
#else
    return {{replaceComponent<VectorIndices::W>(v.components.lo, static_cast<Int_32>(s)), v.components.hi}};
#endif
  }

  template <>
  v_ui32_8 replaceComponent<VectorIndices::X5>(const v_ui32_8& v, const v_ui32_8::ValueType s) noexcept
  {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return replaceComponent<VectorIndices::X5>(v.components, static_cast<Int_32>(s));
#else
    return {{v.components.lo, replaceComponent<VectorIndices::X>(v.components.hi, static_cast<Int_32>(s))}};
#endif
  }

  template <>
  v_ui32_8 replaceComponent<VectorIndices::X6>(const v_ui32_8& v, const v_ui32_8::ValueType s) noexcept
  {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return replaceComponent<VectorIndices::X6>(v.components, static_cast<Int_32>(s));
#else
    return {{v.components.lo, replaceComponent<VectorIndices::Y>(v.components.hi, static_cast<Int_32>(s))}};
#endif
  }

  template <>
  v_ui32_8 replaceComponent<VectorIndices::X7>(const v_ui32_8& v, const v_ui32_8::ValueType s) noexcept
  {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return replaceComponent<VectorIndices::X7>(v.components, static_cast<Int_32>(s));
#else
    return {{v.components.lo, replaceComponent<VectorIndices::Z>(v.components.hi, static_cast<Int_32>(s))}};
#endif
  }

  template <>
  v_ui32_8 replaceComponent<VectorIndices::X8>(const v_ui32_8& v, const v_ui32_8::ValueType s) noexcept
  {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return replaceComponent<VectorIndices::X8>(v.components, static_cast<Int_32>(s));
#else
    return {{v.components.lo, replaceComponent<VectorIndices::W>(v.components.hi, static_cast<Int_32>(s))}};
#endif
  }

  v_ui32_8 replaceX1(const v_ui32_8& v, const v_ui32_8::ValueType s) noexcept
  {
    return replaceComponent<VectorIndices::X1>(v, s);
  }

  v_ui32_8 replaceX2(const v_ui32_8& v, const v_ui32_8::ValueType s) noexcept
  {
    return replaceComponent<VectorIndices::X2>(v, s);
  }

  v_ui32_8 replaceX3(const v_ui32_8& v, const v_ui32_8::ValueType s) noexcept
  {
    return replaceComponent<VectorIndices::X3>(v, s);
  }

  v_ui32_8 replaceX4(const v_ui32_8& v, const v_ui32_8::ValueType s) noexcept
  {
    return replaceComponent<VectorIndices::X4>(v, s);
  }

  v_ui32_8 replaceX5(const v_ui32_8& v, const v_ui32_8::ValueType s) noexcept
  {
    return replaceComponent<VectorIndices::X5>(v, s);
  }

  v_ui32_8 replaceX6(const v_ui32_8& v, const v_ui32_8::ValueType s) noexcept
  {
    return replaceComponent<VectorIndices::X6>(v, s);
  }

  v_ui32_8 replaceX7(const v_ui32_8& v, const v_ui32_8::ValueType s) noexcept
  {
    return replaceComponent<VectorIndices::X7>(v, s);
  }

  v_ui32_8 replaceX8(const v_ui32_8& v, const v_ui32_8::ValueType s) noexcept
  {
    return replaceComponent<VectorIndices::X8>(v, s);
  }
}
