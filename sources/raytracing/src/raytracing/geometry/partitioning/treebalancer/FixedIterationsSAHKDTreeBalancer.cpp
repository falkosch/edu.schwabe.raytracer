#include "raytracing/geometry/partitioning/treebalancer/FixedIterationsSAHKDTreeBalancer.h"
#include "../../../../stdafx.h"

#include <limits>
#include <vector>

namespace raytracer
{
    FixedIterationsSAHKDTreeBalancer::FixedIterationsSAHKDTreeBalancer()
        : geometryNodesSizeThreshold(GeometryNodesSizeThreshold)
    {
    }

    FixedIterationsSAHKDTreeBalancer::FixedIterationsSAHKDTreeBalancer(ASizeT geometryNodesSizeThresholdIn)
        : geometryNodesSizeThreshold(geometryNodesSizeThresholdIn)
    {
    }

    FixedIterationsSAHKDTreeBalancer::~FixedIterationsSAHKDTreeBalancer() = default;

    const KDTreePlane FixedIterationsSAHKDTreeBalancer::
    findSplitter(const KDTreeBuildParameters& parameters, const ASizeT, const AxisAlignedBoundingBox& bounding,
                 const PGeometryNodeList& geometry, const KDTreePlane*)
    const
    {
        const ASizeT maxAxis = argmax3(extents(bounding));

        Float bestCost = std::numeric_limits<Float>::max();
        auto bestPlane = KDTreePlane(Half<Float>(), maxAxis);

        std::vector<Float> candidates;

        if (geometry.size() >= geometryNodesSizeThreshold)
        {
            const Float norm = reciprocal(static_cast<Float>(geometryNodesSizeThreshold + One<ASizeT>()));
            candidates.reserve(geometryNodesSizeThreshold);

            for (ASizeT i = Zero<ASizeT>(); i < geometryNodesSizeThreshold; ++i)
            {
                const Float4 testPosition =
                    mix(bounding.minimum, bounding.maximum, static_cast<Float>(i + One<ASizeT>()) * norm);
                candidates.push_back(testPosition[maxAxis]);
            }
        }
        else
        {
            candidates.reserve(geometry.size());
            for (const auto* geometryNode : geometry)
            {
                candidates.push_back(geometryNodeMaximumPredicate(*geometryNode)[maxAxis]);
            }
        }

        sweepFindBestSplit(parameters, maxAxis, geometry, bounding, candidates, bestCost, bestPlane);

        return bestPlane;
    }
}
