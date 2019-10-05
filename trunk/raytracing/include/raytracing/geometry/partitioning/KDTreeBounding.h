#pragma once

#include "GeometryNode.h"
#include "KDTreePlane.h"

namespace raytracer
{
    using namespace primitives;

    struct KDTreeBounding
    {
        static void split(const KDTreePlane & splitter, const AxisAlignedBoundingBox & toSplit, AxisAlignedBoundingBox & leftBounding, AxisAlignedBoundingBox & rightBounding);

        static const AxisAlignedBoundingBox findMinimumBoundingOfGeometry(const PGeometryNodeList & geometry);
    };
}
