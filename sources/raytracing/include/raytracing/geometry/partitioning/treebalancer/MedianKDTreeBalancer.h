#pragma once

#include "SamplingKDTreeBalancer.h"

namespace raytracer
{
    using namespace vectorization;
    using namespace primitives;

    class MedianKDTreeBalancer : public SamplingKDTreeBalancer
    {
    public:
        MedianKDTreeBalancer();

        ~MedianKDTreeBalancer() override;

        const KDTreePlane findSplitter(
            const KDTreeBuildParameters& parameters, ASizeT treeDepth, const AxisAlignedBoundingBox& bounding,
            const PGeometryNodeList& geometry, const KDTreePlane* parentSplitter
        ) const;
    };
}
