#pragma once

#include "geometry/partitioning/KDTreeBalancer.h"

namespace raytracer
{

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
