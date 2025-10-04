#include "raytracing/geometry/forms/Box.h"
#include "../../../stdafx.h"

namespace raytracer {
  Box::Box() : bounding(oneW(NegativeOne<Float4>()), One<Float4>()) {
  }

  Box::Box(const AxisAlignedBoundingBox &boundingIn) : bounding(boundingIn) {
  }

  Box::Box(const Float4 &min, const Float4 &max) : bounding(min, max) {
  }

  Box::~Box() = default;

  const AxisAlignedBoundingBox Box::getBounding() const {
    return bounding;
  }

  const Float
  computeFacetIntersection(const Float d, const RayCast &rayCast, const Box &b, FacetIntersection &intersectionOut) {
    if (outOfReach(rayCast, d)) {
      return rayCast.maxDistance;
    }

    const Float4 msVertex = point(rayCast.ray, d);
    const Float4 orientation = msVertex - center(b.bounding);
    const Float4 normal = axisAlignedNormal3(orientation);
    intersectionOut.msVertex = msVertex;
    intersectionOut.msSurfaceNormal = normal;
    intersectionOut.vertex = msVertex;
    intersectionOut.surfaceNormal = normal;
    intersectionOut.smoothedNormal = normal;
    intersectionOut.texCoords = mapSpherical(normalize(orientation));
    intersectionOut.node = &b;

    return d;
  }

  const Float Box::findNearestIntersection(
      const RayCast &r, const FacetIntersection *const, FacetIntersection &intersectionOut
  ) const {
    return computeFacetIntersection(
        nearestIntersection(r, bounding, reinterpret_cast<ASizeT>(this)), r, *this, intersectionOut
    );
  }

  const Float
  Box::findAnyIntersection(const RayCast &r, const FacetIntersection *const, FacetIntersection &intersectionOut) const {
    return computeFacetIntersection(
        nearestIntersection(r, bounding, reinterpret_cast<ASizeT>(this)), r, *this, intersectionOut
    );
  }
}