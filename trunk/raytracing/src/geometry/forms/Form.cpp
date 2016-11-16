#include "../../stdafx.h"

#include "geometry/forms/Form.h"

namespace raytracer
{

    Form::~Form() { }

    const bool Form::isInfinite() const
    {
        return false;
    }

    const AxisAlignedBoundingBox Form::getBounding() const
    {
        return AxisAlignedBoundingBox(oneW(NegOne<Float4>()), One<Float4>());
    }

    void Form::includeInBounding(AxisAlignedBoundingBox & aabb) const
    {
        aabb = extendBy(aabb, getBounding());
    }

    const bool Form::overlaps(const AxisAlignedBoundingBox & aabb) const
    {
        return primitives::overlaps(aabb, getBounding());
    }

}
