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

    const ASizeT _mm_popcnt_si128(const PackedInts_128 v) noexcept {
        // see https://stackoverflow.com/a/17355341
        auto v0 = x_64(v);
        auto v1 = x_64(_mm_unpackhi_epi64(v, v));

#ifdef ARCH_X64
#if     defined(__GNUC__)
        return static_cast<ASizeT>(_mm_popcnt_u64(v0) + _mm_popcnt_u64(v1));
#else
        return static_cast<ASizeT>(_mm_popcnt_u64(v0) + _mm_popcnt_u64(v1));
#endif

#else
        auto count = _mm_popcnt_u32(static_cast<UInt_32>(v0 & 0xffffffff))
            + _mm_popcnt_u32(static_cast<UInt_32>(v0 >> 32))
            + _mm_popcnt_u32(static_cast<UInt_32>(v1 & 0xffffffff))
            + _mm_popcnt_u32(static_cast<UInt_32>(v1 >> 32));
        return ASizeT{ count };

#endif
    }

    const ASizeT _mm256_popcnt_si256(const PackedInts_256 v) noexcept {
        return _mm_popcnt_si128(_mm256_castsi256_si128(v))
            + _mm_popcnt_si128(_mm256_extractf128_si256(v, 1));
    }

    const ASizeT _mm_popcnt_pd(const PackedFloat2_128 v) noexcept {
        return _mm_popcnt_si128(_mm_castpd_si128(v));
    }

    const ASizeT _mm_popcnt_ps(const PackedFloat4_128 v) noexcept {
        return _mm_popcnt_si128(_mm_castps_si128(v));
    }

    const ASizeT _mm256_popcnt_ps(const PackedFloat8_256 v) noexcept {
        return _mm256_popcnt_si256(_mm256_castps_si256(v));
    }

    const ASizeT _mm256_popcnt_pd(const PackedFloat4_256 v) noexcept {
        return _mm256_popcnt_si256(_mm256_castpd_si256(v));
    }
}