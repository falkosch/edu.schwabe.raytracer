#include "query_result.h"

#include "counting_leaf_traverser.h"

#include <raytracing/common/StatisticsCookie.h>
#include <raytracing/geometry/forms/meshes/MeshGeometryNode.h>

namespace benchmarks
{
    QueryResult countQuery(
        const Query query, const Mesh& mesh, const KDTreeTraverser<FacetIntersection>& traverser,
        const std::vector<RayCast>& rayCasts
    )
    {
        auto result = QueryResult();
        auto intersection = FacetIntersection();

        auto statistics = StatisticsCookie();
        StatisticsCookie::current = &statistics;
        for (const auto& rayCast : rayCasts)
        {
            const auto distance = trace(query, mesh, rayCast, intersection);
            if (distance < rayCast.maxDistance)
            {
                ++result.hits;
                if (query == Query::Nearest)
                {
                    if (const auto* facetNode = dynamic_cast<const MeshGeometryNode*>(intersection.node))
                    {
                        result.facetIndexSum += facetNode->index;
                    }
                    else
                    {
                        ++result.hitsWithoutFacet;
                    }
                    result.distanceSum += distance;
                }
            }
        }
        StatisticsCookie::current = nullptr;
        result.kdTreeNodesVisited = statistics.kdTreeNodesVisited;

        const auto counting = CountingLeafTraverser(mesh);
        const auto& graph = *mesh.getGraph();
        for (const auto& rayCast : rayCasts)
        {
            if (query == Query::Nearest)
            {
                traverser.findNearestIntersection(counting, graph, rayCast, nullptr, intersection);
            }
            else
            {
                traverser.findAnyIntersection(counting, graph, rayCast, nullptr, intersection);
            }
        }
        result.leafVisits = counting.leafVisits;
        result.facetsInVisitedLeaves = counting.facetsInVisitedLeaves;

        return result;
    }
}
