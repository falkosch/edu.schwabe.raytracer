#pragma once

#include "geometry/partitioning/treebalancer/SAHKDTreeBalancer.h"

namespace raytracer
{

    class BruteForceSAHKDTreeBalancer : public SAHKDTreeBalancer
    {
    public:

        virtual ~BruteForceSAHKDTreeBalancer();

        const KDTreePlane findSplitter(
            const KDTreeBuildParameters & paramaters,
            const ASizeT treeDepth,
            const KDTreeBounding & bounding,
            const PGeometryNodeList & geometry,
            const KDTreePlane * parentSplitter) const;
    };

}
