#pragma once

#include "SamplingKDTreeBalancer.h"

namespace raytracer
{
    using namespace vectorization;
    using namespace primitives;

    class SAHKDTreeBalancer : public SamplingKDTreeBalancer
    {
    public:
        ~SAHKDTreeBalancer() override;

    protected:
        void testSplit(
            const KDTreeBuildParameters& parameters, const Float4& position, ASizeT axis,
            const PGeometryNodeList& geometry, const AxisAlignedBoundingBox& parentBounding, Float& bestCost,
            KDTreePlane& bestPlane
        ) const;

        void sweepFindBestSplit(
            const KDTreeBuildParameters& parameters, ASizeT axis, const PGeometryNodeList& geometry,
            const AxisAlignedBoundingBox& parentBounding, const std::vector<Float>& candidates, Float& bestCost,
            KDTreePlane& bestPlane
        ) const;

        static const AxisAlignedBoundingBox geometryNodeBounding(const GeometryNode& node);
    };
}
