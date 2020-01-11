#pragma once

#include "../../partitioning/GeometryNode.h"

#include <primitives.h>

namespace raytracer
{
    using namespace vectorization;
    using namespace primitives;

    struct MeshGeometryNode : GeometryNode
    {
        ASizeT index;

        Facet facet;

        MeshGeometryNode();

        explicit MeshGeometryNode(const ASizeT index, const Facet & facet);

        // GeometryNode interface

        const AxisAlignedBoundingBox includeInBounding(const AxisAlignedBoundingBox & aabb) const;

        const bool overlaps(const AxisAlignedBoundingBox & aabb) const;

        const Float getIndividualIntersectionCosts() const {
            return 79.1726f;
        }

        // Intersectable<Raycast, FacetIntersection> interface

        const Float findNearestIntersection(const Raycast & ray, const FacetIntersection * const originIntersection, FacetIntersection & intersectionOut) const;

        const Float findAnyIntersection(const Raycast & ray, const FacetIntersection * const originIntersection, FacetIntersection & intersectionOut) const;
    };
}
