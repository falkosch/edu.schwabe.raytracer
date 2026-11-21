#pragma once

#include "query.h"

#include <vector>

namespace benchmarks
{
    using namespace vectorization;
    using namespace primitives;
    using namespace raytracer;

    struct Timing
    {
        // Per ray. Core cycles are TSC cycles scaled by the core clock measured around each repetition.
        // best: sum over the chunks of each chunk's fastest repetition; median/max: whole passes
        double bestCoreCycles{}, medianCoreCycles{}, maxCoreCycles{};
        double medianTscCycles{};
        double medianNanoseconds{};
        // estimated core clock during the timed passes
        double medianCoreGigahertz{};
    };

    // Times the query over the rays: a warm-up pass, then the given number of timed repetitions.
    Timing timeQuery(Query query, const Mesh& mesh, const std::vector<RayCast>& rayCasts, ASizeT repetitions);
}
