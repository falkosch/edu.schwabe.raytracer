#include "vectorization/functions/sqr.h"

namespace vectorization
{
    Int_32 sqr(const Int_32 value) noexcept
    {
        return value * value;
    }

    Int_64 sqr(const Int_64 value) noexcept
    {
        return value * value;
    }

    Float_32 sqr(const Float_32 value) noexcept
    {
        return value * value;
    }

    Float_64 sqr(const Float_64 value) noexcept
    {
        return value * value;
    }

    PackedFloat4_128 sqr(const PackedFloat4_128& values) noexcept
    {
        return _mm_mul_ps(values, values);
    }

    PackedFloat2_128 sqr(const PackedFloat2_128& values) noexcept
    {
        return _mm_mul_pd(values, values);
    }

    PackedFloat8_256 sqr(const PackedFloat8_256& values) noexcept
    {
        return _mm256_mul_ps(values, values);
    }

    PackedFloat4_256 sqr(const PackedFloat4_256& values) noexcept
    {
        return _mm256_mul_pd(values, values);
    }
}
