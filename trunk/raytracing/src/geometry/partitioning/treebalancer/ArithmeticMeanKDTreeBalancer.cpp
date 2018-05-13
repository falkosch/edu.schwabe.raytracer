#include "../../../stdafx.h"

#include "geometry/partitioning/treebalancer/ArithmeticMeanKDTreeBalancer.h"

namespace raytracer
{

    ArithmeticMeanKDTreeBalancer::ArithmeticMeanKDTreeBalancer() : SamplingKDTreeBalancer() { }

    ArithmeticMeanKDTreeBalancer::~ArithmeticMeanKDTreeBalancer() { }

    const KDTreePlane ArithmeticMeanKDTreeBalancer::findSplitter(
        const KDTreeBuildParameters &,
        const ASizeT,
        const AxisAlignedBoundingBox & bounding,
        const PGeometryNodeList & geometry,
        const KDTreePlane *) const
    {
        const Float4 boundingExtents = extents(bounding);
        const ASizeT maxAxis = argmax3(boundingExtents);

        Float sum = Zero<Float>();
        for (PGeometryNodeList::const_iterator it = geometry.cbegin(); it != geometry.cend(); ++it)
        {
            sum += SamplingKDTreeBalancer::geometryNodeMaximumPredicate(**it)[maxAxis];
        }

        return SamplingKDTreeBalancer::sampleSplittingPlane(
            maxAxis,
            bounding.minimum,
            boundingExtents,
            Float4(sum / static_cast<Float>(geometry.size())));
    }

}
