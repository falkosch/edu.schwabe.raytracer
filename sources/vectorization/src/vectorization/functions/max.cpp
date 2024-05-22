#include "vectorization/functions/max.h"

#include "vectorization/accessors.h"
#include "vectorization/selects.h"

namespace vectorization {
  bool max(const bool a, const bool b) noexcept {
    return a | b;
  }

  Int_8 max(const Int_8 a, const Int_8 b) noexcept {
    return select(a > b, a, b);
  }

  UInt_8 max(const UInt_8 a, const UInt_8 b) noexcept {
    return select(a > b, a, b);
  }

  Int_16 max(const Int_16 a, const Int_16 b) noexcept {
    return select(a > b, a, b);
  }

  UInt_16 max(const UInt_16 a, const UInt_16 b) noexcept {
    return select(a > b, a, b);
  }

  Int_32 max(const Int_32 a, const Int_32 b) noexcept {
    return select(a > b, a, b);
  }

  UInt_32 max(const UInt_32 a, const UInt_32 b) noexcept {
    return select(a > b, a, b);
  }

  Int_64 max(const Int_64 a, const Int_64 b) noexcept {
    return select(a > b, a, b);
  }

  UInt_64 max(const UInt_64 a, const UInt_64 b) noexcept {
    return select(a > b, a, b);
  }

  Float_32 max(const Float_32 a, const Float_32 b) noexcept {
    return x(_mm_max_ss(_mm_set_ss(a), _mm_set_ss(b)));
  }

  Float_64 max(const Float_64 a, const Float_64 b) noexcept {
    return x(_mm_max_sd(_mm_set_sd(a), _mm_set_sd(b)));
  }

  PackedFloat4_128 max(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept {
    return _mm_max_ps(a, b);
  }

  PackedFloat2_128 max(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept {
    return _mm_max_pd(a, b);
  }
}
