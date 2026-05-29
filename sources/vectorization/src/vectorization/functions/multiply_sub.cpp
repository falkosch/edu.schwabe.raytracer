#include "vectorization/functions/multiply_sub.h"

#include "vectorization/accessors/component_128d.h"
#include "vectorization/accessors/component_128s.h"
#include "vectorization/functions/multiply.h"
#include "vectorization/functions/subtract.h"

namespace vectorization
{
    Float_32 multiplySub(const Float_32 a, const Float_32 b, const Float_32 c) noexcept
    {
        return x(multiplySubX(_mm_set_ss(a), _mm_set_ss(b), _mm_set_ss(c)));
    }

    Float_64 multiplySub(const Float_64 a, const Float_64 b, const Float_64 c) noexcept
    {
        return x(multiplySubX(_mm_set_sd(a), _mm_set_sd(b), _mm_set_sd(c)));
    }

    PackedFloat4_128 multiplySub(const PackedFloat4_128& a, const PackedFloat4_128& b,
                                 const PackedFloat4_128& c) noexcept
    {
#if VECTORIZATION_FMA
        return _mm_fmsub_ps(a, b, c);
#else
        return subtract(multiply(a, b), c);
#endif
    }

    PackedFloat2_128 multiplySub(const PackedFloat2_128& a, const PackedFloat2_128& b,
                                 const PackedFloat2_128& c) noexcept
    {
#if VECTORIZATION_FMA
        return _mm_fmsub_pd(a, b, c);
#else
        return subtract(multiply(a, b), c);
#endif
    }

    PackedFloat8_256 multiplySub(const PackedFloat8_256& a, const PackedFloat8_256& b,
                                 const PackedFloat8_256& c) noexcept
    {
#if VECTORIZATION_FMA
        return _mm256_fmsub_ps(a, b, c);
#else
        return subtract(multiply(a, b), c);
#endif
    }

    PackedFloat4_256 multiplySub(const PackedFloat4_256& a, const PackedFloat4_256& b,
                                 const PackedFloat4_256& c) noexcept
    {
#if VECTORIZATION_FMA
        return _mm256_fmsub_pd(a, b, c);
#else
        return subtract(multiply(a, b), c);
#endif
    }

    PackedFloat4_128 multiplySubX(const PackedFloat4_128& a, const PackedFloat4_128& b,
                                  const PackedFloat4_128& c) noexcept
    {
#if VECTORIZATION_FMA
        return _mm_fmsub_ss(a, b, c);
#else
        return subtractX(multiplyX(a, b), c);
#endif
    }

    PackedFloat2_128 multiplySubX(const PackedFloat2_128& a, const PackedFloat2_128& b,
                                  const PackedFloat2_128& c) noexcept
    {
#if VECTORIZATION_FMA
        return _mm_fmsub_sd(a, b, c);
#else
        return subtractX(multiplyX(a, b), c);
#endif
    }
}
