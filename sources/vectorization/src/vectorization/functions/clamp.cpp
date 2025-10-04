#include "vectorization/functions/clamp.h"

#include "vectorization/functions/max.h"
#include "vectorization/functions/min.h"

#include "vectorization/accessors.h"

namespace vectorization {
  bool clamp(const bool value, const bool lower, const bool upper) noexcept {
    return max(lower, min(value, upper));
  }

  Int_8 clamp(const Int_8 value, const Int_8 lower, const Int_8 upper) noexcept {
    return max(lower, min(value, upper));
  }

  UInt_8 clamp(const UInt_8 value, const UInt_8 lower, const UInt_8 upper) noexcept {
    return max(lower, min(value, upper));
  }

  Int_16 clamp(const Int_16 value, const Int_16 lower, const Int_16 upper) noexcept {
    return max(lower, min(value, upper));
  }

  UInt_16 clamp(const UInt_16 value, const UInt_16 lower, const UInt_16 upper) noexcept {
    return max(lower, min(value, upper));
  }

  Int_32 clamp(const Int_32 value, const Int_32 lower, const Int_32 upper) noexcept {
    return max(lower, min(value, upper));
  }

  UInt_32 clamp(const UInt_32 value, const UInt_32 lower, const UInt_32 upper) noexcept {
    return max(lower, min(value, upper));
  }

  Int_64 clamp(const Int_64 value, const Int_64 lower, const Int_64 upper) noexcept {
    return max(lower, min(value, upper));
  }

  UInt_64 clamp(const UInt_64 value, const UInt_64 lower, const UInt_64 upper) noexcept {
    return max(lower, min(value, upper));
  }

  Float_32 clamp(const Float_32 value, const Float_32 lower, const Float_32 upper) noexcept {
    return x(_mm_max_ss(_mm_set_ss(lower), _mm_min_ss(_mm_set_ss(value), _mm_set_ss(upper))));
  }

  Float_64 clamp(const Float_64 value, const Float_64 lower, const Float_64 upper) noexcept {
    return x(_mm_max_sd(_mm_set_sd(lower), _mm_min_sd(_mm_set_sd(value), _mm_set_sd(upper))));
  }

  PackedFloat4_128
  clamp(const PackedFloat4_128 &values, const PackedFloat4_128 &lower, const PackedFloat4_128 &upper) noexcept {
    return _mm_max_ps(lower, _mm_min_ps(values, upper));
  }

  PackedFloat2_128
  clamp(const PackedFloat2_128 &values, const PackedFloat2_128 &lower, const PackedFloat2_128 &upper) noexcept {
    return _mm_max_pd(lower, _mm_min_pd(values, upper));
  }
}
