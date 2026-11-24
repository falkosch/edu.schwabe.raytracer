#pragma once

#include <vectorization.h>

namespace raytracer
{
    using namespace vectorization;

    // The weight of a sample in multiple importance sampling (Veach 1997, the power heuristic with the exponent 2): of
    // one of the samples drawn with the density pdf in its direction, against otherSamples of another strategy whose
    // density there is otherPdf. The weights of the two strategies sum to 1 where either density is positive; 0 where
    // pdf is 0. The densities must be finite.
    Float powerHeuristic(Float samples, Float pdf, Float otherSamples, Float otherPdf) noexcept;
}
