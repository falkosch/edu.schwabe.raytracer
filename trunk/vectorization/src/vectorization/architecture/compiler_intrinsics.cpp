#include "vectorization/architecture/compiler_intrinsics.h"

namespace vectorization
{
    const PackedFloat4_128 _mm_cvtepu32_ps(const PackedInts_128 v) {
        // see https://stackoverflow.com/a/34067907
        const PackedInts_128 v2 = _mm_srli_epi32(v, 1); // v2 = v / 2
        return _mm_add_ps(
            _mm_cvtepi32_ps(v2),
            _mm_cvtepi32_ps(_mm_sub_epi32(v, v2)) // v1 = v - (v / 2)
        );
    }
}