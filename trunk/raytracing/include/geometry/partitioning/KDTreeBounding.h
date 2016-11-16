#pragma once

#include "geometry/partitioning/GeometryNode.h"
#include "geometry/partitioning/KDTreePlane.h"

namespace raytracer
{

    struct KDTreeBounding : AxisAlignedBoundingBox
    {
        ALIGNED_ALLOCATORS(__alignof(KDTreeBounding));

        KDTreeBounding();

        KDTreeBounding(const Float4 & min, const Float4 & max);

        KDTreeBounding(const AxisAlignedBoundingBox & box);

        void split(const KDTreePlane & splitter, KDTreeBounding & leftBounding, KDTreeBounding & rightBounding) const;

        static const KDTreeBounding findMinimumBoundingOfGeometry(const PGeometryNodeList & geometry);
    };

}
