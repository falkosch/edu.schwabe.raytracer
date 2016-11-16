#pragma once

#include "geometry/partitioning/treebalancer/SamplingKDTreeBalancer.h"

namespace raytracer
{

    class ArithmeticMeanKDTreeBalancer : public SamplingKDTreeBalancer
    {
    public:

        ArithmeticMeanKDTreeBalancer();

        virtual ~ArithmeticMeanKDTreeBalancer();

        const KDTreePlane findSplitter(
            const KDTreeBuildParameters & parameters,
            const ASizeT treeDepth,
            const KDTreeBounding & bounding,
            const PGeometryNodeList & geometry,
            const KDTreePlane * parentSplitter) const;
    };

}
