#include "primitives/SplittingPlane.h"

using namespace vectorization;

namespace primitives {
  auto SIGN_MASK = blend<false, true, false, true>(NegativeZero<Float4>(), Zero<Float4>());

  SplittingPlane::SplittingPlane() noexcept : normalDistance(OneZ<Float4>()) {
  }

  SplittingPlane::SplittingPlane(const Float4 &normalDistanceIn) noexcept : normalDistance(normalDistanceIn) {
  }

  SplittingPlane::SplittingPlane(const Float4 &origin, const Float4 &normal) noexcept
      : normalDistance(replaceW(normal, dot3(normal, origin))) {
  }

  const AxisAlignedBoundingBox bounding(const SplittingPlane & /* p */) noexcept {
    // infinite planes have infinite bounds
    return AxisAlignedBoundingBox();
  }

  // x = signed distance to plane
  // y = ray dot normal
  // z = -ray dot normal
  // w = ray dot normal
  inline const Float4 computePlaneIntersectionCoefficients(const Ray &r, const SplittingPlane &p) noexcept {
    auto orientation = dot3v(r.direction, p.normalDistance);
    auto signedDistance = dotv(r.origin, p.normalDistance) / orientation;
    return x_yzw(signedDistance, orientation) ^ SIGN_MASK;
  }

  // x = (signed distance to plane <= 0)
  // y = (ray dot normal <= 0)
  // z = (ray dot normal > 0)
  // w = (ray dot normal <= 0)
  inline const Float4::VectorBoolType testPlaneIntersectionCoefficients(const Float4 &c) noexcept {
    return c < Zero<Float4>();
  }

  const bool overlaps(const AxisAlignedBoundingBox &a, const SplittingPlane &p) noexcept {
    auto notNegative = p.normalDistance >= Zero<Float4>();
    auto selmin = oneW(blendMasked(a.maximum, a.minimum, notNegative));
    auto selmax = oneW(blendMasked(a.minimum, a.maximum, notNegative));
    return isNegative(dotv(p.normalDistance, selmin)) & !isNegative(dotv(p.normalDistance, selmax));
  }

  const bool overlaps(const RayCast &r, const SplittingPlane &p) noexcept {
    auto coefficients = computePlaneIntersectionCoefficients(r.ray, p);
    auto check = testPlaneIntersectionCoefficients(coefficients);
    if (x(check | (zwzw(check) & backfaceCulledv(r)) | (yyyy(check) & frontfaceCulledv(r)))) {
      return false;
    }
    return !outOfReach(r, x(coefficients));
  }

  const Float nearestIntersection(const RayCast &r, const SplittingPlane &p, const Size2::ValueType originId) noexcept {
    auto coefficients = computePlaneIntersectionCoefficients(r.ray, p);
    auto check = testPlaneIntersectionCoefficients(coefficients);
    if (x(check | (zwzw(check) & backfaceCulledv(r)) | (yyyy(check) & frontfaceCulledv(r)))) {
      return r.maxDistance;
    }
    return select(selfOcclusion(r, originId, x(coefficients)), r.maxDistance, x(coefficients));
  }
}