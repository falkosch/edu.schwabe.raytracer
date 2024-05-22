#include "vectorization/functions/isNaN.h"

namespace vectorization {
  bool isNaN(const Float_32 value) noexcept {
    const auto t = _mm_set_ss(value);
    return !!_mm_cvtsi128_si32(_mm_castps_si128(_mm_cmpunord_ss(t, t)));
  }

  bool isNaN(const Float_64 value) noexcept {
    const auto t = _mm_set_sd(value);
    return !!_mm_cvtsi128_si32(_mm_castpd_si128(_mm_cmpunord_sd(t, t)));
  }

  PackedFloat4_128 isNaN(const PackedFloat4_128 &values) noexcept {
    return _mm_cmpunord_ps(values, values);
  }

  PackedFloat2_128 isNaN(const PackedFloat2_128 &values) noexcept {
    return _mm_cmpunord_pd(values, values);
  }

  bool isNaN(const Float_32 a, const Float_32 b) noexcept {
    return !!_mm_cvtsi128_si32(_mm_castps_si128(_mm_cmpunord_ss(_mm_set_ss(a), _mm_set_ss(b))));
  }

  bool isNaN(const Float_64 a, const Float_64 b) noexcept {
    return !!_mm_cvtsi128_si32(_mm_castpd_si128(_mm_cmpunord_sd(_mm_set_sd(a), _mm_set_sd(b))));
  }

  PackedFloat4_128 isNaN(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept {
    return _mm_cmpunord_ps(a, b);
  }

  PackedFloat2_128 isNaN(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept {
    return _mm_cmpunord_pd(a, b);
  }
}
