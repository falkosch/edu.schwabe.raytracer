#include "vectorization/functions/divide.h"

#include "vectorization/accessors.h"

namespace vectorization {
  Int_8 divide(const Int_8 a, const Int_8 b) noexcept {
    return static_cast<Int_8>(a / b);
  }

  UInt_8 divide(const UInt_8 a, const UInt_8 b) noexcept {
    return static_cast<UInt_8>(a / b);
  }

  Int_16 divide(const Int_16 a, const Int_16 b) noexcept {
    return static_cast<Int_16>(a / b);
  }

  UInt_16 divide(const UInt_16 a, const UInt_16 b) noexcept {
    return static_cast<UInt_16>(a / b);
  }

  Int_32 divide(const Int_32 a, const Int_32 b) noexcept {
    return a / b;
  }

  UInt_32 divide(const UInt_32 a, const UInt_32 b) noexcept {
    return a / b;
  }

  Int_64 divide(const Int_64 a, const Int_64 b) noexcept {
    return a / b;
  }

  UInt_64 divide(const UInt_64 a, const UInt_64 b) noexcept {
    return a / b;
  }

  Float_32 divide(const Float_32 a, const Float_32 b) noexcept {
#ifdef VECTORIZATION_APPROXIMATIONS
#ifdef VECTORIZATION_FINE_APPROXIMATIONS
    const auto two = Two<PackedFloat4_128>();
    const auto bs = _mm_set_ss(b);
    const auto x0 = _mm_rcp_ss(bs);
    const auto x1 = _mm_mul_ss(_mm_sub_ss(two, _mm_mul_ss(bs, x0)), x0);
    return x(_mm_mul_ss(_mm_set_ss(a), _mm_mul_ss(_mm_sub_ss(two, _mm_mul_ss(bs, x1)), x1)));
#else
    return x(_mm_mul_ss(_mm_set_ss(a), _mm_rcp_ss(_mm_set_ss(b))));
#endif
#else
    return x(_mm_div_ss(_mm_set_ss(a), _mm_set_ss(b)));
#endif
  }

  Float_64 divide(const Float_64 a, const Float_64 b) noexcept {
    return a / b;
  }

  PackedFloat4_128 divide(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept {
#ifdef VECTORIZATION_APPROXIMATIONS
#ifdef VECTORIZATION_FINE_APPROXIMATIONS
    const auto two = Two<PackedFloat4_128>();
    const auto x0 = _mm_rcp_ps(b);
    const auto x1 = _mm_mul_ps(_mm_sub_ps(two, _mm_mul_ps(b, x0)), x0);
    return _mm_mul_ps(a, _mm_mul_ps(_mm_sub_ps(two, _mm_mul_ps(b, x1)), x1));
#else
    return _mm_mul_ps(a, _mm_rcp_ps(b));
#endif
#else
    return _mm_div_ps(a, b);
#endif
  }

  PackedFloat2_128 divide(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept {
    return _mm_div_pd(a, b);
  }
}
