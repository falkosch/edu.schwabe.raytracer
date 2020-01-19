#pragma once

#include "SAHKDTreeBalancer.h"

namespace raytracer
{
    using namespace vectorization;
    using namespace primitives;

    class BruteForceSAHKDTreeBalancer
        : public SAHKDTreeBalancer
    {
    public:

        virtual ~BruteForceSAHKDTreeBalancer();

        const KDTreePlane findSplitter(
            const KDTreeBuildParameters & paramaters,
            const ASizeT treeDepth,
            const AxisAlignedBoundingBox & bounding,
            const PGeometryNodeList & geometry,
            const KDTreePlane * parentSplitter
        ) const;
    };
}
