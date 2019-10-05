#pragma once

#include "SamplingKDTreeBalancer.h"

namespace raytracer
{
    using namespace vectorization;
    using namespace primitives;

    class SAHKDTreeBalancer
        : public SamplingKDTreeBalancer
    {
    public:

        virtual ~SAHKDTreeBalancer();

    protected:

        void testSplit(
            const KDTreeBuildParameters & parameters,
            const Float4 & position,
            const ASizeT axis,
            const PGeometryNodeList & geometry,
            const AxisAlignedBoundingBox & parentBounding,
            Float & bestCost,
            KDTreePlane & bestPlane
        ) const;

        static const AxisAlignedBoundingBox geometryNodeBounding(const GeometryNode & node);
    };
}
