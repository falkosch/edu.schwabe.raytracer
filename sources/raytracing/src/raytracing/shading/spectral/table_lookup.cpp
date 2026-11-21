#include "raytracing/shading/spectral/table_lookup.h"

#include "raytracing/shading/spectral/wavelengths.h"

#include <vectorization.h>

namespace raytracer::spectral
{
    TableCoordinates tableCoordinates(const Float8& wavelengths, const Float tableMin, const Float tableStep,
                                      const ASizeT tableSize) noexcept
    {
        const auto lastIndex = static_cast<Float>(tableSize - ASizeT{1});
        const auto clamped = clamp(wavelengths, Float8(tableMin), Float8(tableMin + tableStep * lastIndex));
        const auto index = (clamped - Float8(tableMin)) / Float8(tableStep);
        // The last grid entry is reached as the upper end of the last interval, with fraction 1.
        const auto lowerIndex = min(floor(index), Float8(lastIndex - One<Float>()));
        return TableCoordinates{lowerIndex, index - lowerIndex};
    }

    Float8 lookupTable(const Float* table, const TableCoordinates& coordinates) noexcept
    {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
        const auto indices = _mm256_cvtps_epi32(coordinates.index.components);
        const auto indicesPlus1 = _mm256_add_epi32(indices, _mm256_set1_epi32(1));
        const Float8 lo{_mm256_i32gather_ps(table, indices, sizeof(Float))};
        const Float8 hi{_mm256_i32gather_ps(table, indicesPlus1, sizeof(Float))};
#else
        Float8 lo, hi;
        for (auto i = ASizeT{0}; i < SPECTRAL_SAMPLES; ++i)
        {
            const auto ix = static_cast<ASizeT>(coordinates.index[i]);
            lo[i] = table[ix];
            hi[i] = table[ix + ASizeT{1}];
        }
#endif

        return mix(lo, hi, coordinates.fraction);
    }
}
