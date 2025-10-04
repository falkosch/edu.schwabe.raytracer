#include "vectorization/conversions/complex.h"

#include "vectorization/selects.h"

namespace vectorization {
  template <>
  v_i32_4 convert<v_i32_4>(const bool v) noexcept {
    return v_i32_4(boolMask<Int_32>(v));
  }

  template <>
  v_ui32_4 convert<v_ui32_4>(const bool v) noexcept {
    return convert<v_i32_4>(v).components;
  }

  template <>
  v_i32_4 convert<v_i32_4>(const bool a, const bool b) noexcept {
    return v_i32_4(boolMask<Int_32>(a), boolMask<Int_32>(b));
  }

  template <>
  v_ui32_4 convert<v_ui32_4>(const bool a, const bool b) noexcept {
    return convert<v_i32_4>(a, b).components;
  }

  template <>
  v_f32_4 convert<v_f32_4>(const v_ui32_4 &v) noexcept {
    return vectorization::_mm_cvtepu32_ps(v.components);
  }

  template <>
  v_f32_4 convert<v_f32_4>(const v_i32_4 &v) noexcept {
    return _mm_cvtepi32_ps(v.components);
  }

  template <>
  v_ui64_2 convert<v_ui64_2>(const v_i32_4 &v) noexcept {
    return _mm_cvtepu32_epi64(v.components);
  }

  template <>
  v_i32_4 convert<v_i32_4>(const v_f32_4 &v) noexcept {
    return _mm_cvtps_epi32(v.components);
  }

  template <>
  v_f32_4 convert<v_f32_4>(const v_f32_4 &v) noexcept {
    return v;
  }

  template <>
  v_ui64_2 convert<v_ui64_2>(const v_f32_4 &v) noexcept {
#ifdef ARCH_X64
    const auto x = static_cast<Int_64>(_mm_cvtss_si64(v.components));
    const auto y = static_cast<Int_64>(_mm_cvtss_si64(yyww(v).components));
    return v_ui64_2(convert<UInt_64>(x), convert<UInt_64>(y));
#else
    return v_ui64_2(convert<UInt_64>(x(v)), convert<UInt_64>(y(v)));
#endif
  }

  template <>
  v_f32_4 convert<v_f32_4>(const v_ui64_2 &v) noexcept {
    return v_f32_4(convert<Float_32>(x(v)), convert<Float_32>(y(v)));
  }
}
