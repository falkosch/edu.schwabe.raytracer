#include "../../stdafx.h"

#include "geometry/partitioning/KDTreeBounding.h"

#include <thread>

namespace raytracer
{

    KDTreeBounding::KDTreeBounding() : AxisAlignedBoundingBox() { }

    KDTreeBounding::KDTreeBounding(const Float4 & min, const Float4 & max)
    {
        minimum = min;
        maximum = max;
    }

    KDTreeBounding::KDTreeBounding(const AxisAlignedBoundingBox & box)
        :
        AxisAlignedBoundingBox(box)
    { }

    void KDTreeBounding::split(
        const KDTreePlane & splitter,
        KDTreeBounding & leftBounding,
        KDTreeBounding & rightBounding) const
    {
        leftBounding = KDTreeBounding(minimum, splitter.splitMax(minimum, maximum));
        rightBounding = KDTreeBounding(splitter.splitMin(minimum, maximum), maximum);
    }

    const KDTreeBounding KDTreeBounding::findMinimumBoundingOfGeometry(const PGeometryNodeList & geometry)
    {
        const int geometrySize = static_cast<int>(geometry.size());
        KDTreeBounding bounding = KDTreeBounding();

#pragma omp parallel if (geometrySize >= int(std::thread::hardware_concurrency()))
        {
            AxisAlignedBoundingBox t = AxisAlignedBoundingBox();

#pragma omp for nowait
            for (int i = Zero<int>(); i < geometrySize; ++i)
            {
                geometry[static_cast<ASizeT>(i)]->includeInBounding(t);
            }

#pragma omp critical (mergeBoundings)
            {
                bounding = extendBy(t, bounding);
            }
        }

        return bounding;
    }

}
