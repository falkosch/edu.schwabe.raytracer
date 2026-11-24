#pragma once

#include <vectorization.h>

namespace raytracer
{
    using namespace vectorization;

    // The spread of a pixel's sub-samples for adaptive sampling (RaytraceBaseParameters::adaptiveRounds): the count,
    // sum and sum of squares of their luminance as display values
    struct SubSampleSpread
    {
        Float count;
        Float sum;
        Float sumOfSquares;

        SubSampleSpread();

        // A sub-sample of the color (sRGB-linear)
        void add(const Float4& rgb) noexcept;

        // The standard error of the sub-samples' mean, 0 for fewer than two
        Float error() const noexcept;

        // The luminance of the color (sRGB-linear) as a display value: compressed like the shoulder of a filmic view
        // transform (y / (1 + y)) and encoded with the exponent 1 / 2.2, in [0, 1)
        static Float displayValue(const Float4& rgb) noexcept;
    };
}
