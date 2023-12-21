#include "raytracing/geometry/forms/Sphere.h"
#include "../../../stdafx.h"

#include <cassert>

namespace raytracer {
  Sphere::Sphere() : bounding(OneW<Float4>(), One<Float>()) {
  }

  Sphere::Sphere(const BoundingSphere &boundingSphere) : bounding(boundingSphere) {
  }

  Sphere::Sphere(const Float4 &center, const Float radius) : bounding(center, radius) {
    assert(radius > Zero<Float>());
  }

  Sphere::~Sphere() = default;

  const AxisAlignedBoundingBox Sphere::getBounding() const {
    return primitives::bounding(bounding);
  }

  const Float
  computeFacetIntersection(const Float d, const RayCast &rayCast, const Sphere &s, FacetIntersection &intersectionOut) {
    if (outOfReach(rayCast, d)) {
      return rayCast.maxDistance;
    }
    const Float4 msVertex = point(rayCast.ray, d);
    const Float4 normal = normalize3(msVertex - s.bounding.centerRadius);
    intersectionOut.msVertex = msVertex;
    intersectionOut.msSurfaceNormal = normal;
    intersectionOut.vertex = msVertex;
    intersectionOut.surfaceNormal = normal;
    intersectionOut.smoothedNormal = normal;
    intersectionOut.texCoords = mapSpherical(normal);
    intersectionOut.node = &s;
    return d;
  }

  const Float Sphere::findNearestIntersection(
      const RayCast &rayCast, const FacetIntersection *const, FacetIntersection &intersectionOut
  ) const {
    return computeFacetIntersection(
        nearestIntersection(rayCast, bounding, reinterpret_cast<ASizeT>(this)), rayCast, *this, intersectionOut
    );
  }

  const Float Sphere::findAnyIntersection(
      const RayCast &rayCast, const FacetIntersection *const, FacetIntersection &intersectionOut
  ) const {
    return computeFacetIntersection(
        nearestIntersection(rayCast, bounding, reinterpret_cast<ASizeT>(this)), rayCast, *this, intersectionOut
    );
  }
}