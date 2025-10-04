#include "raytracing/geometry/forms/Plane.h"
#include "../../../stdafx.h"

namespace raytracer {
  Plane::Plane() : plane(), baseVectors() {
    reconstructFacetEdgesFromPlaneEquation(plane.normalDistance);
  }

  Plane::Plane(const Float4 &origin, const Float4 &normal) : plane(origin, normal), baseVectors() {
    reconstructFacetEdgesFromPlaneEquation(plane.normalDistance);
  }

  Plane::Plane(const Float4 &origin, const FacetEdges &baseVectorsIn)
      : plane(origin, cross3(baseVectorsIn.edge1, baseVectorsIn.edge2)), baseVectors(baseVectorsIn) {
  }

  Plane::Plane(const Float4 &planeEquation) : plane(planeEquation), baseVectors() {
    reconstructFacetEdgesFromPlaneEquation(planeEquation);
  }

  Plane::~Plane() = default;

  bool Plane::isInfinite() const {
    return true;
  }

  void Plane::reconstructFacetEdgesFromPlaneEquation(const Float4 &planeEquation) {
    // choose an axis that is not linear or co-linear to plane-normal
    // and construct a perpendicular vector with it
    const Float4 normal = zeroW(planeEquation);
    const Float4 base0 = cross3(
        normal, select(One<Float4>() - xxxx(normal * normal) < Epsilon<Float4>(), OneX<Float4>(), OneY<Float4>())
    );
    baseVectors.edge1 = base0;
    // find another perpendicular vector
    baseVectors.edge2 = cross3(normal, base0);
  }

  Float
  computeFacetIntersection(const Float d, const RayCast &rayCast, const Plane &p, FacetIntersection &intersectionOut) {
    if (outOfReach(rayCast, d)) {
      return rayCast.maxDistance;
    }

    const Float4 msVertex = point(rayCast.ray, d);
    const Float4 normal = zeroW(p.plane.normalDistance);
    intersectionOut.msVertex = msVertex;
    intersectionOut.msSurfaceNormal = normal;
    intersectionOut.vertex = msVertex;
    intersectionOut.surfaceNormal = normal;
    intersectionOut.smoothedNormal = normal;
    intersectionOut.texCoords = mapOrthogonal(msVertex, p.baseVectors.edge1, p.baseVectors.edge2);
    intersectionOut.node = &p;
    return d;
  }

  Float Plane::findNearestIntersection(
      const RayCast &rayCast, const FacetIntersection *const, FacetIntersection &intersectionOut
  ) const {
    return computeFacetIntersection(
        nearestIntersection(rayCast, plane, reinterpret_cast<ASizeT>(this)), rayCast, *this, intersectionOut
    );
  }

  Float Plane::findAnyIntersection(
      const RayCast &rayCast, const FacetIntersection *const, FacetIntersection &intersectionOut
  ) const {
    return computeFacetIntersection(
        nearestIntersection(rayCast, plane, reinterpret_cast<ASizeT>(this)), rayCast, *this, intersectionOut
    );
  }
}
