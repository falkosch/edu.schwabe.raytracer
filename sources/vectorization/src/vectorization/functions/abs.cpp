#include "vectorization/functions/abs.h"

#include "vectorization/accessors.h"
#include "vectorization/constants.h"
#include "vectorization/selects.h"

#include <cassert>
#include <limits>

namespace vectorization {
  Int_8 abs(const Int_8 value) noexcept {
    assert(value != std::numeric_limits<Int_8>::min() && "abs of minimum signed value is undefined");
    return select(value < Zero<Int_8>(), static_cast<Int_8>(-value), value);
  }

  Int_16 abs(const Int_16 value) noexcept {
    assert(value != std::numeric_limits<Int_16>::min() && "abs of minimum signed value is undefined");
    return select(value < Zero<Int_16>(), static_cast<Int_16>(-value), value);
  }

  Int_32 abs(const Int_32 value) noexcept {
    assert(value != std::numeric_limits<Int_32>::min() && "abs of minimum signed value is undefined");
    return std::abs(value);
  }

  Int_64 abs(const Int_64 value) noexcept {
    assert(value != std::numeric_limits<Int_64>::min() && "abs of minimum signed value is undefined");
    return std::llabs(value);
  }

  Float_32 abs(const Float_32 value) noexcept {
    return x(abs(_mm_set_ss(value)));
  }

  Float_64 abs(const Float_64 value) noexcept {
    return x(abs(_mm_set_sd(value)));
  }

  PackedFloat4_128 abs(const PackedFloat4_128 &values) noexcept {
    return _mm_andnot_ps(NegativeZero<PackedFloat4_128>(), values);
  }

  PackedFloat2_128 abs(const PackedFloat2_128 &values) noexcept {
    return _mm_andnot_pd(NegativeZero<PackedFloat2_128>(), values);
  }
}
