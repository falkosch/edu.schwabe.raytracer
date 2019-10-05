#include "raytracing/geometry/partitioning/KDTreePlane.h"
#include "../../../stdafx.h"

namespace raytracer
{
    KDTreePlane::KDTreePlane()
        :
        t(),
        splitAxis() { }

    KDTreePlane::KDTreePlane(
        const Float tIn,
        const ASizeT splitAxisIn)
        :
        t(tIn),
        splitAxis(splitAxisIn) { }

    const Float4 KDTreePlane::splitMax(const Float4 & min, const Float4 & max) const {
        Float4 split = max;
        split[splitAxis] = mix(min[splitAxis], max[splitAxis], t);
        return split;
    }

    const Float4 KDTreePlane::splitMin(const Float4 & min, const Float4 & max) const {
        Float4 split = min;
        split[splitAxis] = mix(min[splitAxis], max[splitAxis], t);
        return split;
    }

    const Float4 KDTreePlane::splitMax(const AxisAlignedBoundingBox & aabb) const {
        return splitMax(aabb.minimum, aabb.maximum);
    }

    const Float4 KDTreePlane::splitMin(const AxisAlignedBoundingBox & aabb) const {
        return splitMin(aabb.minimum, aabb.maximum);
    }
}