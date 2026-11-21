#include "leaf_statistics.h"

#include <algorithm>

namespace benchmarks
{
    namespace
    {
        ASizeT roundUp(const ASizeT value, const ASizeT multiple)
        {
            return (value + multiple - 1) / multiple * multiple;
        }
    }

    void collectLeafStatistics(const KDTreeRoot& graph, const KDTreeLink link, LeafStatistics& statistics)
    {
        if (!KDTreeRoot::isLeaf(link))
        {
            collectLeafStatistics(graph, graph.childLinks[link].childA, statistics);
            collectLeafStatistics(graph, graph.childLinks[link].childB, statistics);
            return;
        }

        ++statistics.leaves;
        const auto size = graph.geometryOf(link).size();
        if (size == 0)
        {
            ++statistics.emptyLeaves;
            return;
        }

        statistics.references += size;
        statistics.minSize = std::min(statistics.minSize, size);
        statistics.maxSize = std::max(statistics.maxSize, size);
        statistics.paddedReferences4 += roundUp(size, 4);
        statistics.paddedReferences8 += roundUp(size, 8);

        auto bucket = ASizeT{0};
        for (auto upper = ASizeT{4}; bucket < statistics.histogram.size() - 1 && size > upper; upper *= 2)
        {
            ++bucket;
        }
        ++statistics.histogram[bucket];
    }
}
