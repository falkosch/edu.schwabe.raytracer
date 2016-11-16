#pragma once

#include "geometry/partitioning/treebalancer/SAHKDTreeBalancer.h"

namespace raytracer
{

    class FixedIterationsSAHKDTreeBalancer : public SAHKDTreeBalancer
    {
    public:

        static const ASizeT GeometryNodesSizeThreshold = ASizeT(25);

        FixedIterationsSAHKDTreeBalancer();

        FixedIterationsSAHKDTreeBalancer(const ASizeT geometryNodesSizeThreshold);

        virtual ~FixedIterationsSAHKDTreeBalancer();

        const KDTreePlane findSplitter(
            const KDTreeBuildParameters & paramaters,
            const ASizeT treeDepth,
            const KDTreeBounding & bounding,
            const PGeometryNodeList & geometry,
            const KDTreePlane * parentSplitter) const;

    protected:

        ASizeT geometryNodesSizeThreshold;
    };

}
