#pragma once

#include "geometry/partitioning/treebalancer/SamplingKDTreeBalancer.h"

namespace raytracer
{

    class SAHKDTreeBalancer : public SamplingKDTreeBalancer
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
            KDTreePlane & bestPlane) const;

        const AxisAlignedBoundingBox geometryNodeBounding(const GeometryNode & node) const;
    };

}
