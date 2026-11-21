#pragma once

#include <raytracing/geometry/partitioning/KDTreeRoot.h>

#include <array>
#include <limits>

namespace benchmarks
{
    using namespace vectorization;
    using namespace raytracer;

    struct LeafStatistics
    {
        ASizeT leaves{}, emptyLeaves{}, references{};
        ASizeT minSize{std::numeric_limits<ASizeT>::max()}, maxSize{};
        ASizeT paddedReferences4{}, paddedReferences8{};
        // leaf sizes 1-4, 5-8, 9-16, 17-32, 33-64, >64
        std::array<ASizeT, 6> histogram{};
    };

    // walks the links from the root, so only reachable leaves count
    void collectLeafStatistics(const KDTreeRoot& graph, KDTreeLink link, LeafStatistics& statistics);
}
