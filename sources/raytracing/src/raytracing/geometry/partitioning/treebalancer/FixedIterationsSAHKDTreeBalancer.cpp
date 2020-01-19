#include "raytracing/geometry/partitioning/treebalancer/FixedIterationsSAHKDTreeBalancer.h"
#include "../../../../stdafx.h"

#include <algorithm>

namespace raytracer
{
    FixedIterationsSAHKDTreeBalancer::FixedIterationsSAHKDTreeBalancer()
        :
        geometryNodesSizeThreshold(GeometryNodesSizeThreshold) { }

    FixedIterationsSAHKDTreeBalancer::FixedIterationsSAHKDTreeBalancer(const ASizeT geometryNodesSizeThresholdIn)
        :
        geometryNodesSizeThreshold(geometryNodesSizeThresholdIn) { }

    FixedIterationsSAHKDTreeBalancer::~FixedIterationsSAHKDTreeBalancer() { }

    const KDTreePlane FixedIterationsSAHKDTreeBalancer::findSplitter(
        const KDTreeBuildParameters & paramaters,
        const ASizeT,
        const AxisAlignedBoundingBox & bounding,
        const PGeometryNodeList & geometry,
        const KDTreePlane *
    ) const {
        // get the usual geometry stuff
        const ASizeT maxAxis = argmax3(extents(bounding));

        // remember the best split and its associated costs
        // initialize it with the mid-point and assume worst costs for it
        Float bestCost = std::numeric_limits<Float>::max();
        KDTreePlane bestPlane = KDTreePlane(Half<Float>(), maxAxis);

        // approximate a good splitting plane by searching through fixed iterations,
        // but if geometry-list is too small, switch over to a brute-force approach
        if (geometry.size() >= geometryNodesSizeThreshold) {
            const Float norm = reciprocal(static_cast<Float>(geometryNodesSizeThreshold + One<ASizeT>()));

            for (ASizeT i = Zero<ASizeT>(); i < geometryNodesSizeThreshold; ++i) {
                const Float4 testPosition = mix(bounding.minimum, bounding.maximum, static_cast<Float>(i + One<ASizeT>()) * norm);
                testSplit(paramaters, testPosition, maxAxis, geometry, bounding, bestCost, bestPlane);
            }
        } else {
            std::for_each(geometry.cbegin(), geometry.cend(), [&](auto geometryNode) {
                const Float4 testPosition = SamplingKDTreeBalancer::geometryNodeMaximumPredicate(*geometryNode);
                testSplit(paramaters, testPosition, maxAxis, geometry, bounding, bestCost, bestPlane);
            });
        }

        // finally, use the saved best splitting
        return bestPlane;
    }
}