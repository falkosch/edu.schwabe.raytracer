#include "raytracing/shading/spectral/wavelengths.h"

#include <vectorization.h>

namespace raytracer::spectral
{
    static const Float8 LANE_OFFSETS{
        Zero<Float>(), DELTA_LAMBDA, DELTA_LAMBDA * 2, DELTA_LAMBDA * 3,
        DELTA_LAMBDA * 4, DELTA_LAMBDA * 5, DELTA_LAMBDA * 6, DELTA_LAMBDA * 7
    };

    Float wrapToVisible(const Float lambda) noexcept
    {
        return LAMBDA_MIN + modulo(lambda - LAMBDA_MIN, LAMBDA_RANGE);
    }

    Float8 wavelengthsAt(const Float heroLambda) noexcept
    {
        const auto lambdaMin = Float8(LAMBDA_MIN);
        const auto raw = Float8(heroLambda) + LANE_OFFSETS - lambdaMin;
        return modulo(raw, Float8(LAMBDA_RANGE)) + lambdaMin;
    }
}
