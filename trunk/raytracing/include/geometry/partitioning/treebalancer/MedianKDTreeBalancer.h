#pragma once

#include "geometry/partitioning/treebalancer/SamplingKDTreeBalancer.h"

namespace raytracer
{

    class MedianKDTreeBalancer : public SamplingKDTreeBalancer
    {
    public:

        MedianKDTreeBalancer();

        virtual ~MedianKDTreeBalancer();

        const KDTreePlane findSplitter(
            const KDTreeBuildParameters & parameters,
            const ASizeT treeDepth,
            const AxisAlignedBoundingBox & bounding,
            const PGeometryNodeList & geometry,
            const KDTreePlane * parentSplitter) const;
    };

}
