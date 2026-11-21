#pragma once

#include <raytracing/geometry/FacetIntersection.h>
#include <raytracing/geometry/forms/meshes/Mesh.h>
#include <raytracing/geometry/partitioning/GeometryNodesTraverser.h>

namespace benchmarks
{
    using namespace vectorization;
    using namespace primitives;
    using namespace raytracer;

    // Counts what the leaf loops get to see, then forwards to the mesh's own leaf intersection. Only used in the
    // untimed counting pass, so the timed path stays the production one.
    class CountingLeafTraverser final : public GeometryNodesTraverser<FacetIntersection>
    {
    public:
        explicit CountingLeafTraverser(const Mesh& mesh);

        Float findNearestIntersection(
            GeometryNodeSpan geometryNodes, const RayCast& rayCast, const FacetIntersection* originIntersection,
            FacetIntersection& intersectionOut
        ) const override;

        Float findAnyIntersection(
            GeometryNodeSpan geometryNodes, const RayCast& rayCast, const FacetIntersection* originIntersection,
            FacetIntersection& intersectionOut
        ) const override;

        mutable ASizeT leafVisits{};
        mutable ASizeT facetsInVisitedLeaves{};

    private:
        const Mesh& mesh;
    };
}
