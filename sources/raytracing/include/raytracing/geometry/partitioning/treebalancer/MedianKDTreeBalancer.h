#pragma once

#include "SamplingKDTreeBalancer.h"

namespace raytracer
{
    using namespace vectorization;
    using namespace primitives;

    class MedianKDTreeBalancer
        : public SamplingKDTreeBalancer
    {
    public:

        MedianKDTreeBalancer();

        virtual ~MedianKDTreeBalancer();

        const KDTreePlane findSplitter(
            const KDTreeBuildParameters & parameters,
            const ASizeT treeDepth,
            const AxisAlignedBoundingBox & bounding,
            const PGeometryNodeList & geometry,
            const KDTreePlane * parentSplitter
        ) const;
    };
}
