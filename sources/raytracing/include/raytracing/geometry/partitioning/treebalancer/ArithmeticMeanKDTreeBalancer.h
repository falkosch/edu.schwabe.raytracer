#pragma once

#include "SamplingKDTreeBalancer.h"

namespace raytracer
{
    using namespace vectorization;
    using namespace primitives;

    class ArithmeticMeanKDTreeBalancer : public SamplingKDTreeBalancer
    {
    public:
        ArithmeticMeanKDTreeBalancer();

        ~ArithmeticMeanKDTreeBalancer() override;

        const KDTreePlane findSplitter(
            const KDTreeBuildParameters& parameters, ASizeT treeDepth, const AxisAlignedBoundingBox& bounding,
            const PGeometryNodeList& geometry, const KDTreePlane* parentSplitter
        ) const;
    };
}
