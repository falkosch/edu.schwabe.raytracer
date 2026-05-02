#pragma once

#include "SAHKDTreeBalancer.h"

namespace raytracer
{
    using namespace vectorization;
    using namespace primitives;

    class FixedIterationsSAHKDTreeBalancer : public SAHKDTreeBalancer
    {
    public:
        static constexpr ASizeT GeometryNodesSizeThreshold = 25;

        FixedIterationsSAHKDTreeBalancer();

        FixedIterationsSAHKDTreeBalancer(ASizeT geometryNodesSizeThreshold);

        ~FixedIterationsSAHKDTreeBalancer() override;

        const KDTreePlane findSplitter(
            const KDTreeBuildParameters& parameters, ASizeT treeDepth, const AxisAlignedBoundingBox& bounding,
            const PGeometryNodeList& geometry, const KDTreePlane* parentSplitter
        ) const;

    protected:
        ASizeT geometryNodesSizeThreshold;
    };
}
