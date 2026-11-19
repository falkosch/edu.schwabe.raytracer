#include "raytracing/shading/spectral/wavelengths.h"

#include <vectorization.h>

namespace raytracer::spectral
{
    static const Float8 LANE_OFFSETS{
        Zero<Float>(), DELTA_LAMBDA, DELTA_LAMBDA * 2, DELTA_LAMBDA * 3,
        DELTA_LAMBDA * 4, DELTA_LAMBDA * 5, DELTA_LAMBDA * 6, DELTA_LAMBDA * 7
    };

    // A reassociating compiler (clang's -fassociative-math) may distribute the division inside modulo(), which can
    // put the floor() argument just below or above an integer at the range boundaries (780 -> 780 instead of 380).
    // Fold such results back into [LAMBDA_MIN, LAMBDA_MAX).
    Float wrapToVisible(const Float lambda) noexcept
    {
        const auto wrapped = LAMBDA_MIN + modulo(lambda - LAMBDA_MIN, LAMBDA_RANGE);
        if (wrapped >= LAMBDA_MAX)
            return wrapped - LAMBDA_RANGE;
        if (wrapped < LAMBDA_MIN)
            return wrapped + LAMBDA_RANGE;
        return wrapped;
    }

    Float8 wavelengthsAt(const Float heroLambda) noexcept
    {
        const auto lambdaMin = Float8(LAMBDA_MIN);
        const auto lambdaMax = Float8(LAMBDA_MAX);
        const auto range = Float8(LAMBDA_RANGE);
        const auto raw = Float8(heroLambda) + LANE_OFFSETS - lambdaMin;
        const auto wrapped = modulo(raw, range) + lambdaMin;
        const auto belowMax = select(wrapped >= lambdaMax, wrapped - range, wrapped);
        return select(belowMax < lambdaMin, belowMax + range, belowMax);
    }
}
