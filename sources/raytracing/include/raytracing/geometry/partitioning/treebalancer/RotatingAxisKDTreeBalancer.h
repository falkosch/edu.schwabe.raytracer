#pragma once

#include "../KDTreeBalancer.h"

namespace raytracer
{
    using namespace vectorization;
    using namespace primitives;

    class RotatingAxisKDTreeBalancer : public KDTreeBalancer
    {
    public:
        static constexpr ASizeT D = 3;

        RotatingAxisKDTreeBalancer();

        ~RotatingAxisKDTreeBalancer() override;

        const KDTreePlane findSplitter(
            const KDTreeBuildParameters& parameters, ASizeT treeDepth, const AxisAlignedBoundingBox& bounding,
            const PGeometryNodeList& geometry, const KDTreePlane* parentSplitter
        ) const;
    };
}
