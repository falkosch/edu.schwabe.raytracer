#include "counting_leaf_traverser.h"

namespace benchmarks
{
    CountingLeafTraverser::CountingLeafTraverser(const Mesh& mesh) : mesh(mesh)
    {
    }

    Float CountingLeafTraverser::findNearestIntersection(
        const GeometryNodeSpan geometryNodes, const RayCast& rayCast, const FacetIntersection* originIntersection,
        FacetIntersection& intersectionOut
    ) const
    {
        ++leafVisits;
        facetsInVisitedLeaves += geometryNodes.size();
        return mesh.findNearestIntersection(geometryNodes, rayCast, originIntersection, intersectionOut);
    }

    Float CountingLeafTraverser::findAnyIntersection(
        const GeometryNodeSpan geometryNodes, const RayCast& rayCast, const FacetIntersection* originIntersection,
        FacetIntersection& intersectionOut
    ) const
    {
        ++leafVisits;
        facetsInVisitedLeaves += geometryNodes.size();
        return mesh.findAnyIntersection(geometryNodes, rayCast, originIntersection, intersectionOut);
    }
}
