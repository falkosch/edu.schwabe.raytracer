#include "raytracing/geometry/forms/Form.h"
#include "../../../stdafx.h"

namespace raytracer {
  Form::~Form() = default;

  bool Form::isInfinite() const {
    return false;
  }

  AxisAlignedBoundingBox Form::getBounding() const {
    return AxisAlignedBoundingBox(oneW(NegativeOne<Float4>()), One<Float4>());
  }

  AxisAlignedBoundingBox Form::includeInBounding(const AxisAlignedBoundingBox &aabb) const {
    return extendBy(aabb, getBounding());
  }

  bool Form::overlaps(const AxisAlignedBoundingBox &aabb) const {
    return primitives::overlaps(aabb, getBounding());
  }
}
