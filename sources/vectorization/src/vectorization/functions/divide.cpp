#include "vectorization/functions/divide.h"

#include "vectorization/accessors/component_128d.h"
#include "vectorization/accessors/component_128s.h"
#include "vectorization/functions/multiply.h"
#include "vectorization/functions/negative_multiply_add.h"

#include <cassert>

namespace vectorization {
  Int_8 divide(const Int_8 a, const Int_8 b) noexcept {
    assert(b != 0 && "integer division by zero");
    return static_cast<Int_8>(a / b);
  }

  UInt_8 divide(const UInt_8 a, const UInt_8 b) noexcept {
    assert(b != 0 && "integer division by zero");
    return static_cast<UInt_8>(a / b);
  }

  Int_16 divide(const Int_16 a, const Int_16 b) noexcept {
    assert(b != 0 && "integer division by zero");
    return static_cast<Int_16>(a / b);
  }

  UInt_16 divide(const UInt_16 a, const UInt_16 b) noexcept {
    assert(b != 0 && "integer division by zero");
    return static_cast<UInt_16>(a / b);
  }

  Int_32 divide(const Int_32 a, const Int_32 b) noexcept {
    assert(b != 0 && "integer division by zero");
    return a / b;
  }

  UInt_32 divide(const UInt_32 a, const UInt_32 b) noexcept {
    assert(b != 0 && "integer division by zero");
    return a / b;
  }

  Int_64 divide(const Int_64 a, const Int_64 b) noexcept {
    assert(b != 0 && "integer division by zero");
    return a / b;
  }

  UInt_64 divide(const UInt_64 a, const UInt_64 b) noexcept {
    assert(b != 0 && "integer division by zero");
    return a / b;
  }

  Float_32 divide(const Float_32 a, const Float_32 b) noexcept {
#ifdef VECTORIZATION_APPROXIMATIONS
#ifdef VECTORIZATION_FINE_APPROXIMATIONS
    const auto two = Two<PackedFloat4_128>();
    const auto bs = _mm_set_ss(b);
    const auto x0 = _mm_rcp_ss(bs);
    const auto x1 = multiplyX(negativeMultiplyAddX(bs, x0, two), x0);
    return x(multiplyX(_mm_set_ss(a), multiplyX(negativeMultiplyAddX(bs, x1, two), x1)));
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
    const auto x1 = multiply(negativeMultiplyAdd(b, x0, two), x0);
    return multiply(a, multiply(negativeMultiplyAdd(b, x1, two), x1));
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

  PackedFloat8_256 divide(const PackedFloat8_256 &a, const PackedFloat8_256 &b) noexcept {
#ifdef VECTORIZATION_APPROXIMATIONS
#ifdef VECTORIZATION_FINE_APPROXIMATIONS
    const auto two = Two<PackedFloat8_256>();
    const auto x0 = _mm256_rcp_ps(b);
    const auto x1 = multiply(negativeMultiplyAdd(b, x0, two), x0);
    return multiply(a, multiply(negativeMultiplyAdd(b, x1, two), x1));
#else
    return _mm256_mul_ps(a, _mm256_rcp_ps(b));
#endif
#else
    return _mm256_div_ps(a, b);
#endif
  }

  PackedFloat4_256 divide(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept {
    return _mm256_div_pd(a, b);
  }
}
