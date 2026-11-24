#pragma once

#include <vectorization.h>

#include <vector>

namespace raytracer
{
    using namespace vectorization;

    // The further rounds of adaptive sampling (RaytraceBaseParameters::adaptiveRounds), by the spread of the
    // neighbours' first rounds: every pixel's first round of sub-samples sets its standard error
    // (SubSampleSpread::error), then each pixel takes as many further rounds as bring the error expected of its mean
    // below the threshold. That error comes from the neighbours only, the root mean square of theirs (the spread of the
    // image region), never from the pixel's own sub-samples: a rule that judges a pixel by the samples it averages
    // stops where they missed rare bright rays and refines where they found them, so heavy-tailed noise comes out too
    // dark.
    class AdaptiveRounds
    {
    public:
        explicit AdaptiveRounds(const Size2& resolution);

        // The standard error of the first round of the pixel at the image index (row by row)
        void setFirstRoundError(ASizeT imageIndex, Float error) noexcept;

        // The further rounds of the pixel (x, y): the fewest after which its neighbours' error, divided by sqrt(k + 1)
        // for k rounds, is at most the threshold, up to maxRounds; 0 where the neighbours don't spread or without any
        ASizeT roundsOf(const Size2& pixel, ASizeT maxRounds, Float threshold) const noexcept;

    private:
        Size2 resolution;
        std::vector<Float> firstRoundErrors;
    };
}
