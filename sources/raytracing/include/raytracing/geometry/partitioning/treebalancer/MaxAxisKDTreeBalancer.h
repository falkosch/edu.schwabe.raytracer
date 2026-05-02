#pragma once

#include "../KDTreeBalancer.h"

namespace raytracer
{
    using namespace vectorization;
    using namespace primitives;

    class MaxAxisKDTreeBalancer : public KDTreeBalancer
    {
    public:
        MaxAxisKDTreeBalancer();

        ~MaxAxisKDTreeBalancer() override;

        const KDTreePlane findSplitter(
            const KDTreeBuildParameters& parameters, ASizeT treeDepth, const AxisAlignedBoundingBox& bounding,
            const PGeometryNodeList& geometry, const KDTreePlane* parentSplitter
        ) const;
    };
}
