#include "vectorization/architecture/compiler_intrinsics.h"

#include <array>

namespace vectorization
{
    const PackedFloat4_128 _mm_cvtepu32_ps(const PackedInts_128 v) noexcept {
        // see https://stackoverflow.com/a/34067907
        const PackedInts_128 v2 = _mm_srli_epi32(v, 1); // v2 = v / 2
        return _mm_add_ps(
            _mm_cvtepi32_ps(v2),
            _mm_cvtepi32_ps(_mm_sub_epi32(v, v2)) // v1 = v - (v / 2)
        );
    }

    const UInt_64 x_64(const PackedInts_128 v) noexcept {
#ifdef ARCH_X64
        return static_cast<UInt_64>(_mm_cvtsi128_si64(v));

#else
        union PackBits
        {
            PackedInts_128 value;
            struct
            {
                UInt_64 x, y;
            } bits;

            PackBits(const PackedInts_128 value) : value(value) { }
        } pack{ v };
        return pack.bits.x;

#endif
    }

    const ASizeT __popcntx(const PackedInts_128 v) noexcept {
        // see https://stackoverflow.com/a/17355341
        auto v0 = x_64(v);
        auto v1 = x_64(_mm_unpackhi_epi64(v, v));

#ifdef ARCH_X64
#if     defined(__GNUC__)
        return static_cast<size_t>(__popcntq(v0) + __popcntq(v1));
#else
        return static_cast<size_t>(__popcnt64(v0) + __popcnt64(v1));
#endif

#else
        return __popcnt(v0 & 0xffffffff) + __popcnt(v0 >> 32)
            + __popcnt(v1 & 0xffffffff) + __popcnt(v1 >> 32);

#endif
    }

    const ASizeT __popcntx(const PackedFloat4_128 v) noexcept {
        return __popcntx(_mm_castps_si128(v));
    }

    const ASizeT __popcntx(const PackedFloat2_128 v) noexcept {
        return __popcntx(_mm_castpd_si128(v));
    }

    const ASizeT __popcntx(const PackedInts_256 v) noexcept {
        return __popcntx(_mm256_castsi256_si128(v))
            + __popcntx(_mm256_extractf128_si256(v, 1));
    }

    const ASizeT __popcntx(const PackedFloat8_256 v) noexcept {
        return __popcntx(_mm256_castps_si256(v));
    }

    const ASizeT __popcntx(const PackedFloat4_256 v) noexcept {
        return __popcntx(_mm256_castpd_si256(v));
    }
}