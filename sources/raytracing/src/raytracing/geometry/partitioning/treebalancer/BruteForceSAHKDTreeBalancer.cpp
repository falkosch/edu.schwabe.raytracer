#include "raytracing/geometry/partitioning/treebalancer/BruteForceSAHKDTreeBalancer.h"
#include "../../../../stdafx.h"

#include <limits>
#include <vector>

namespace raytracer
{
    BruteForceSAHKDTreeBalancer::~BruteForceSAHKDTreeBalancer() = default;

    const KDTreePlane BruteForceSAHKDTreeBalancer::
    findSplitter(const KDTreeBuildParameters& parameters, ASizeT, const AxisAlignedBoundingBox& bounding,
                 const PGeometryNodeList& geometry, const KDTreePlane*)
    const
    {
        const auto maxAxis = argmax3(extents(bounding));

        auto bestCost = std::numeric_limits<Float>::max();
        auto bestPlane = KDTreePlane(Half<Float>(), maxAxis);

        std::vector<Float> candidates;
        candidates.reserve(geometry.size() * 3);
        for (const auto* node : geometry)
        {
            const auto nodeBounding = geometryNodeBox(*node);
            candidates.push_back(nodeBounding.minimum[maxAxis]);
            candidates.push_back(center(nodeBounding)[maxAxis]);
            candidates.push_back(nodeBounding.maximum[maxAxis]);
        }

        sweepFindBestSplit(parameters, maxAxis, geometry, bounding, candidates, bestCost, bestPlane);

        return bestPlane;
    }
}
