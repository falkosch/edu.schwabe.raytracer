#pragma once

#include "../KDTreeBalancer.h"

namespace raytracer
{
    using namespace vectorization;
    using namespace primitives;

    class RotatingAxisKDTreeBalancer
        : public KDTreeBalancer
    {
    public:

        static const ASizeT D = 3;

        RotatingAxisKDTreeBalancer();

        virtual ~RotatingAxisKDTreeBalancer();

        const KDTreePlane findSplitter(
            const KDTreeBuildParameters & parameters,
            const ASizeT treeDepth,
            const AxisAlignedBoundingBox & bounding,
            const PGeometryNodeList & geometry,
            const KDTreePlane * parentSplitter
        ) const;
    };
}
