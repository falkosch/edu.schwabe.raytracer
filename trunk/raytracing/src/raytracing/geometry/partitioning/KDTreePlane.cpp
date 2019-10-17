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
        const auto split = mix(min, max, t);
        return replaceComponent(max, split, splitAxis);
    }

    const Float4 KDTreePlane::splitMin(const Float4 & min, const Float4 & max) const {
        const auto split = mix(min, max, t);
        return replaceComponent(min, split, splitAxis);
    }

    const Float4 KDTreePlane::splitMax(const AxisAlignedBoundingBox & aabb) const {
        return splitMax(aabb.minimum, aabb.maximum);
    }

    const Float4 KDTreePlane::splitMin(const AxisAlignedBoundingBox & aabb) const {
        return splitMin(aabb.minimum, aabb.maximum);
    }
}