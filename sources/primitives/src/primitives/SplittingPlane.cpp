#include "primitives/SplittingPlane.h"

using namespace vectorization;

namespace primitives {
  auto SIGN_MASK = blend<false, true, false, true>(NegativeZero<Float4>(), Zero<Float4>());

  SplittingPlane::SplittingPlane() noexcept : normalDistance(OneZ<Float4>()) {
  }

  SplittingPlane::SplittingPlane(const Float4 &normalDistance) noexcept : normalDistance(normalDistance) {
  }

  SplittingPlane::SplittingPlane(const Float4 &origin, const Float4 &normal) noexcept
      : normalDistance(replaceW(normal, dot3(normal, origin))) {
  }

  AxisAlignedBoundingBox bounding(const SplittingPlane & /* p */) noexcept {
    // infinite planes have infinite bounds
    return {};
  }

  // x = signed distance to plane
  // y = ray dot normal
  // z = -ray dot normal
  // w = ray dot normal
  inline Float4 computePlaneIntersectionCoefficients(const Ray &ray, const SplittingPlane &plane) noexcept {
    const auto orientation = dot3v(ray.direction, plane.normalDistance);
    const auto signedDistance = dotv(ray.origin, plane.normalDistance) / orientation;
    return x_yzw(signedDistance, orientation) ^ SIGN_MASK;
  }

  // x = (signed distance to plane <= 0)
  // y = (ray dot normal <= 0)
  // z = (ray dot normal > 0)
  // w = (ray dot normal <= 0)
  inline Float4::VectorBoolType testPlaneIntersectionCoefficients(const Float4 &coefficients) noexcept {
    return coefficients < Zero<Float4>();
  }

  bool overlaps(const AxisAlignedBoundingBox &box, const SplittingPlane &plane) noexcept {
    const auto notNegative = plane.normalDistance >= Zero<Float4>();
    const auto selmin = oneW(blendMasked(box.maximum, box.minimum, notNegative));
    const auto selmax = oneW(blendMasked(box.minimum, box.maximum, notNegative));
    return isNegative(dotv(plane.normalDistance, selmin)) & !isNegative(dotv(plane.normalDistance, selmax));
  }

  bool overlaps(const RayCast &rayCast, const SplittingPlane &plane) noexcept {
    const auto coefficients = computePlaneIntersectionCoefficients(rayCast.ray, plane);
    const auto distanceAndFaceCulling = testPlaneIntersectionCoefficients(coefficients);
    const auto notVisibleBack = zwzw(distanceAndFaceCulling) & backfaceCulledv(rayCast);

    if (const auto notVisibleFront = yyyy(distanceAndFaceCulling) & frontfaceCulledv(rayCast);
        x(distanceAndFaceCulling | notVisibleBack | notVisibleFront)) {

      return false;
    }

    return !outOfReach(rayCast, x(coefficients));
  }

  Float
  nearestIntersection(const RayCast &rayCast, const SplittingPlane &plane, const Size2::ValueType originId) noexcept {
    const auto coefficients = computePlaneIntersectionCoefficients(rayCast.ray, plane);
    const auto check = testPlaneIntersectionCoefficients(coefficients);
    const auto notVisibleBack = zwzw(check) & backfaceCulledv(rayCast);

    if (const auto notVisibleFront = yyyy(check) & frontfaceCulledv(rayCast);
        x(check | notVisibleBack | notVisibleFront)) {

      return rayCast.maxDistance;
    }

    return select(selfOcclusion(rayCast, originId, x(coefficients)), rayCast.maxDistance, x(coefficients));
  }
}
