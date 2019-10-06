#include "vectorization/functions/modulo.h"

#include "vectorization/functions/divide.h"
#include "vectorization/functions/floor.h"

namespace vectorization
{
    const Int_8 modulo(const Int_8 a, const Int_8 b) noexcept {
        return static_cast<Int_8>(a % b);
    }

    const UInt_8 modulo(const UInt_8 a, const UInt_8 b) noexcept {
        return static_cast<UInt_8>(a % b);
    }

    const Int_16 modulo(const Int_16 a, const Int_16 b) noexcept {
        return static_cast<Int_16>(a % b);
    }

    const UInt_16 modulo(const UInt_16 a, const UInt_16 b) noexcept {
        return static_cast<UInt_16>(a % b);
    }

    const Int_32 modulo(const Int_32 a, const Int_32 b) noexcept {
        return a % b;
    }

    const UInt_32 modulo(const UInt_32 a, const UInt_32 b) noexcept {
        return a % b;
    }

    const Int_64 modulo(const Int_64 a, const Int_64 b) noexcept {
        return a % b;
    }

    const UInt_64 modulo(const UInt_64 a, const UInt_64 b) noexcept {
        return a % b;
    }

    const Float_32 modulo(const Float_32 a, const Float_32 b) noexcept {
        return a - b * floor(divide(a, b));
    }

    const Float_64 modulo(const Float_64 a, const Float_64 b) noexcept {
        return a - b * floor(divide(a, b));
    }

    const PackedFloat4_128 modulo(const PackedFloat4_128 & a, const PackedFloat4_128 & b) noexcept {
        return _mm_sub_ps(a, _mm_mul_ps(b, floor(divide(a, b))));
    }

    const PackedFloat2_128 modulo(const PackedFloat2_128 & a, const PackedFloat2_128 & b) noexcept {
        return _mm_sub_pd(a, _mm_mul_pd(b, floor(divide(a, b))));
    }
}