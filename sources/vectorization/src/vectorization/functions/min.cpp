#include "vectorization/functions/min.h"

#include "vectorization/accessors.h"
#include "vectorization/selects.h"

namespace vectorization {
  const bool min(const bool a, const bool b) noexcept {
    return a & b;
  }

  const Int_8 min(const Int_8 a, const Int_8 b) noexcept {
    return select(a < b, a, b);
  }

  const UInt_8 min(const UInt_8 a, const UInt_8 b) noexcept {
    return select(a < b, a, b);
  }

  const Int_16 min(const Int_16 a, const Int_16 b) noexcept {
    return select(a < b, a, b);
  }

  const UInt_16 min(const UInt_16 a, const UInt_16 b) noexcept {
    return select(a < b, a, b);
  }

  const Int_32 min(const Int_32 a, const Int_32 b) noexcept {
    return select(a < b, a, b);
  }

  const UInt_32 min(const UInt_32 a, const UInt_32 b) noexcept {
    return select(a < b, a, b);
  }

  const Int_64 min(const Int_64 a, const Int_64 b) noexcept {
    return select(a < b, a, b);
  }

  const UInt_64 min(const UInt_64 a, const UInt_64 b) noexcept {
    return select(a < b, a, b);
  }

  const Float_32 min(const Float_32 a, const Float_32 b) noexcept {
    return x(_mm_min_ss(_mm_set_ss(a), _mm_set_ss(b)));
  }

  const Float_64 min(const Float_64 a, const Float_64 b) noexcept {
    return x(_mm_min_sd(_mm_set_sd(a), _mm_set_sd(b)));
  }

  const PackedFloat4_128 min(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept {
    return _mm_min_ps(a, b);
  }

  const PackedFloat2_128 min(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept {
    return _mm_min_pd(a, b);
  }
}