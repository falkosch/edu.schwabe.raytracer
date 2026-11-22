#include "raytracing/geometry/forms/Box.h"
#include "../../../stdafx.h"

namespace raytracer {
  Box::Box() : Box(BoxTextureMapping::PerFace) {
  }

  Box::Box(const BoxTextureMapping textureMappingIn)
      : bounding(oneW(NegativeOne<Float4>()), One<Float4>()), textureMapping(textureMappingIn) {
  }

  Box::Box(const AxisAlignedBoundingBox &boundingIn)
      : bounding(boundingIn), textureMapping(BoxTextureMapping::PerFace) {
  }

  Box::Box(const Float4 &min, const Float4 &max) : bounding(min, max), textureMapping(BoxTextureMapping::PerFace) {
  }

  Box::~Box() = default;

  AxisAlignedBoundingBox Box::getBounding() const {
    return bounding;
  }

  // The face's own coordinates in [0, 1], upright as seen from outside (BoxTextureMapping::PerFace)
  Float4 faceCoordinates(const Float4 &msVertex, const Float4 &normal, const AxisAlignedBoundingBox &bounding) {
    const auto relative = (msVertex - bounding.minimum) / max(extents(bounding), Epsilon<Float4>());
    const auto rx = clamp(x(relative), Zero<Float>(), One<Float>());
    const auto ry = clamp(y(relative), Zero<Float>(), One<Float>());
    const auto rz = clamp(z(relative), Zero<Float>(), One<Float>());
    auto u = rx, v = ry;
    if (x(normal) != Zero<Float>()) {
      u = x(normal) > Zero<Float>() ? One<Float>() - rz : rz; // seen from +x, right is -z
    } else if (y(normal) != Zero<Float>()) {
      v = y(normal) > Zero<Float>() ? One<Float>() - rz : rz; // the top face with -z up, the bottom face with +z up
    } else if (z(normal) < Zero<Float>()) {
      u = One<Float>() - rx; // seen from -z, right is -x
    }
    return Float4(u, v, Zero<Float>(), One<Float>());
  }

  Float
  computeFacetIntersection(const Float d, const RayCast &rayCast, const Box &b, FacetIntersection &intersectionOut) {
    if (outOfReach(rayCast, d)) {
      return rayCast.maxDistance;
    }

    const Float4 msVertex = point(rayCast.ray, d);
    const Float4 orientation = msVertex - center(b.bounding);
    // relative to the half lengths, the largest component tells the face also for boxes that aren't cubes
    const Float4 normal = axisAlignedNormal3(orientation / max(halfLengths(b.bounding), Epsilon<Float4>()));
    intersectionOut.msVertex = msVertex;
    intersectionOut.msSurfaceNormal = normal;
    intersectionOut.vertex = msVertex;
    intersectionOut.surfaceNormal = normal;
    intersectionOut.smoothedNormal = normal;
    intersectionOut.texCoords = b.textureMapping == BoxTextureMapping::Spherical
                                    ? mapSpherical(normalize(orientation))
                                    : faceCoordinates(msVertex, normal, b.bounding);
    intersectionOut.node = &b;

    return d;
  }

  Float Box::findNearestIntersection(
      const RayCast &rayCast, const FacetIntersection *const, FacetIntersection &intersectionOut
  ) const {
    return computeFacetIntersection(
        nearestIntersection(rayCast, bounding, reinterpret_cast<ASizeT>(this)), rayCast, *this, intersectionOut
    );
  }

  Float Box::findAnyIntersection(
      const RayCast &rayCast, const FacetIntersection *const, FacetIntersection &intersectionOut
  ) const {
    return computeFacetIntersection(
        nearestIntersection(rayCast, bounding, reinterpret_cast<ASizeT>(this)), rayCast, *this, intersectionOut
    );
  }
}
