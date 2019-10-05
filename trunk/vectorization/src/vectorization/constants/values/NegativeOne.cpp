#include "vectorization/constants/values/NegativeOne.h"

#include "vectorization/constants/values/One.h"
#include "vectorization/constants/values/NegativeZero.h"

namespace vectorization
{
    template <typename T>
    const T NegativeOne() noexcept {
        return { -1 };
    }

    template const Int_8 NegativeOne<Int_8>() noexcept;
    template const Int_16 NegativeOne<Int_16>() noexcept;
    template const Int_32 NegativeOne<Int_32>() noexcept;
    template const Int_64 NegativeOne<Int_64>() noexcept;

    template <>
    const PackedFloat4_128 NegativeOne<PackedFloat4_128>() noexcept {
        return _mm_or_ps(One<PackedFloat4_128>(), NegativeZero<PackedFloat4_128>());
    }

    template <>
    const PackedFloat2_128 NegativeOne<PackedFloat2_128>() noexcept {
        return _mm_or_pd(One<PackedFloat2_128>(), NegativeZero<PackedFloat2_128>());
    }

    template <>
    const Float_32 NegativeOne<Float_32>() noexcept {
        return _mm_cvtss_f32(NegativeOne<PackedFloat4_128>());
    }

    template <>
    const Float_64 NegativeOne<Float_64>() noexcept {
        return _mm_cvtsd_f64(NegativeOne<PackedFloat2_128>());
    }
}