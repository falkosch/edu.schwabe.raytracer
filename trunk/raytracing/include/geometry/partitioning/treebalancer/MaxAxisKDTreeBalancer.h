#pragma once

#include "geometry/partitioning/KDTreeBalancer.h"

namespace raytracer
{

    class MaxAxisKDTreeBalancer : public KDTreeBalancer
    {
    public:

        MaxAxisKDTreeBalancer();

        virtual ~MaxAxisKDTreeBalancer();

        const KDTreePlane findSplitter(
            const KDTreeBuildParameters & parameters,
            const ASizeT treeDepth,
            const KDTreeBounding & bounding,
            const PGeometryNodeList & geometry,
            const KDTreePlane * parentSplitter) const;
    };

}
