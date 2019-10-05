#pragma once

#include "../partitioning/GeometryNode.h"

#include <primitives.h>

namespace raytracer
{
    using namespace primitives;

    class Form : public GeometryNode
    {
    public:

        virtual ~Form();

        virtual const bool isInfinite() const;

        virtual const AxisAlignedBoundingBox getBounding() const;

        virtual void includeInBounding(AxisAlignedBoundingBox & aabb) const;

        virtual const bool overlaps(const AxisAlignedBoundingBox & aabb) const;
    };
}
