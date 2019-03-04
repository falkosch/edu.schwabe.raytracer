#pragma once

#include "SAHKDTreeBalancer.h"

namespace raytracer
{
	using namespace vectorization;
	using namespace primitives;

    class FixedIterationsSAHKDTreeBalancer
		: public SAHKDTreeBalancer
    {
    public:

        static const ASizeT GeometryNodesSizeThreshold = ASizeT(25);

        FixedIterationsSAHKDTreeBalancer();

        FixedIterationsSAHKDTreeBalancer(const ASizeT geometryNodesSizeThreshold);

        virtual ~FixedIterationsSAHKDTreeBalancer();

        const KDTreePlane findSplitter(
            const KDTreeBuildParameters & paramaters,
            const ASizeT treeDepth,
            const AxisAlignedBoundingBox & bounding,
            const PGeometryNodeList & geometry,
            const KDTreePlane * parentSplitter
		) const;

    protected:

        ASizeT geometryNodesSizeThreshold;
    };

}
