#pragma once

#include "geometry/partitioning/KDTreeRoot.h"
#include "geometry/partitioning/GeometryNodesTraverser.h"

namespace raytracer
{

    template <typename IntersectionInfoType>
    class KDTreeTraverser
    {
    public:

        virtual ~KDTreeTraverser() { }

        virtual const Float findNearestIntersection(
            const GeometryNodesTraverser<IntersectionInfoType> & geometryNodesTraverser,
            const KDTreeRoot & root,
            const Raycast & raycast,
            const IntersectionInfoType * const originIntersection,
            IntersectionInfoType & intersectionOut) const = 0;

        virtual const Float findAnyIntersection(
            const GeometryNodesTraverser<IntersectionInfoType> & geometryNodesTraverser,
            const KDTreeRoot & root,
            const Raycast & raycast,
            const IntersectionInfoType * const originIntersection,
            IntersectionInfoType & intersectionOut) const = 0;
    };

}
