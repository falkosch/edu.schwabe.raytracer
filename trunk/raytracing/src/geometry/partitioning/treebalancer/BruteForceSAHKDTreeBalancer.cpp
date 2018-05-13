#include "../../../stdafx.h"

#include "geometry/partitioning/treebalancer/BruteForceSAHKDTreeBalancer.h"

namespace raytracer
{

    BruteForceSAHKDTreeBalancer::~BruteForceSAHKDTreeBalancer() { }

    const KDTreePlane BruteForceSAHKDTreeBalancer::findSplitter(
        const KDTreeBuildParameters & paramaters,
        const ASizeT,
        const AxisAlignedBoundingBox & bounding,
        const PGeometryNodeList & geometry,
        const KDTreePlane *) const
    {
        // get the geometry stuff
        const Float4 parentExtents = extents(bounding);
        const ASizeT maxAxis = argmax3(parentExtents);

        // remember the best split and its associated costs
        // initialize it with the mid-point and assume worst costs for it
        Float bestCost = Float_Limits::max();
        KDTreePlane bestPlane = KDTreePlane(Half<Float>(), maxAxis);

        for (PGeometryNodeList::const_iterator it = geometry.cbegin(); it != geometry.cend(); ++it)
        {
            const AxisAlignedBoundingBox nodeBounding = SamplingKDTreeBalancer::geometryNodeBox(**it);
            testSplit(paramaters, nodeBounding.minimum, maxAxis, geometry, bounding, bestCost, bestPlane);
            testSplit(paramaters, center(nodeBounding), maxAxis, geometry, bounding, bestCost, bestPlane);
            testSplit(paramaters, nodeBounding.maximum, maxAxis, geometry, bounding, bestCost, bestPlane);
        }

        // finally, use the saved best splitting
        return bestPlane;
    }

}
