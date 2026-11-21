#pragma once

#include "query.h"

#include <raytracing/geometry/partitioning/KDTreeTraverser.h>

#include <vector>

namespace benchmarks
{
    using namespace vectorization;
    using namespace primitives;
    using namespace raytracer;

    struct QueryResult
    {
        ASizeT hits{};
        // nearest only: which facet was hit and at which distance; any-hit results depend on the traversal order
        ASizeT facetIndexSum{};
        ASizeT hitsWithoutFacet{};
        double distanceSum{};
        ASizeT kdTreeNodesVisited{};
        ASizeT leafVisits{};
        ASizeT facetsInVisitedLeaves{};
    };

    // Untimed: results through the production entry point (plus KD-tree node visits), then the leaf counters through
    // the counting wrapper.
    QueryResult countQuery(
        Query query, const Mesh& mesh, const KDTreeTraverser<FacetIntersection>& traverser,
        const std::vector<RayCast>& rayCasts
    );
}
