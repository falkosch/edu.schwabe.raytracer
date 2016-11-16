#pragma once

#include "geometry/partitioning/GeometryNode.h"

#include <primitives/AxisAlignedBoundingBox.h>

namespace raytracer
{

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
