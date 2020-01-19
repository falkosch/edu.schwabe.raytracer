#include "vectorization/functions/clamp.h"

#include "vectorization/functions/max.h"
#include "vectorization/functions/min.h"

#include "vectorization/accessors.h"

namespace vectorization
{
    const bool clamp(const bool v, const bool lower, const bool upper) noexcept {
        return max(lower, min(v, upper));
    }

    const Int_8 clamp(const Int_8 v, const Int_8 lower, const Int_8 upper) noexcept {
        return max(lower, min(v, upper));
    }

    const UInt_8 clamp(const UInt_8 v, const UInt_8 lower, const UInt_8 upper) noexcept {
        return max(lower, min(v, upper));
    }

    const Int_16 clamp(const Int_16 v, const Int_16 lower, const Int_16 upper) noexcept {
        return max(lower, min(v, upper));
    }

    const UInt_16 clamp(const UInt_16 v, const UInt_16 lower, const UInt_16 upper) noexcept {
        return max(lower, min(v, upper));
    }

    const Int_32 clamp(const Int_32 v, const Int_32 lower, const Int_32 upper) noexcept {
        return max(lower, min(v, upper));
    }

    const UInt_32 clamp(const UInt_32 v, const UInt_32 lower, const UInt_32 upper) noexcept {
        return max(lower, min(v, upper));
    }

    const Int_64 clamp(const Int_64 v, const Int_64 lower, const Int_64 upper) noexcept {
        return max(lower, min(v, upper));
    }

    const UInt_64 clamp(const UInt_64 v, const UInt_64 lower, const UInt_64 upper) noexcept {
        return max(lower, min(v, upper));
    }

    const Float_32 clamp(const Float_32 v, const Float_32 lower, const Float_32 upper) noexcept {
        return x(
            _mm_max_ss(_mm_set_ss(lower), _mm_min_ss(_mm_set_ss(v), _mm_set_ss(upper)))
        );
    }

    const Float_64 clamp(const Float_64 v, const Float_64 lower, const Float_64 upper) noexcept {
        return x(
            _mm_max_sd(_mm_set_sd(lower), _mm_min_sd(_mm_set_sd(v), _mm_set_sd(upper)))
        );
    }

    const PackedFloat4_128 clamp(const PackedFloat4_128 & v, const PackedFloat4_128 & lower, const PackedFloat4_128 & upper) noexcept {
        return _mm_max_ps(lower, _mm_min_ps(v, upper));
    }

    const PackedFloat2_128 clamp(const PackedFloat2_128 & v, const PackedFloat2_128 & lower, const PackedFloat2_128 & upper) noexcept {
        return _mm_max_pd(lower, _mm_min_pd(v, upper));
    }
}