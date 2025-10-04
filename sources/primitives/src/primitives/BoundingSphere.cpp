#include "primitives/BoundingSphere.h"

using namespace vectorization;

namespace primitives {
  BoundingSphere::BoundingSphere() noexcept : centerRadius(OneW<Float4>()) {
  }

  BoundingSphere::BoundingSphere(const Float4 &center, const Float radius) noexcept
      : centerRadius(replaceW(center, radius)) {
  }

  AxisAlignedBoundingBox bounding(const BoundingSphere &sphere) noexcept {
    const auto radius = wwww(sphere.centerRadius);
    return AxisAlignedBoundingBox(oneW(sphere.centerRadius - radius), oneW(sphere.centerRadius + radius));
  }

  inline Float4 computeSphereIntersectionCoefficients(const Ray &ray, const BoundingSphere &sphere) noexcept {
    // geometric method from Real-Time Rendering 3: http://books.google.de/books?id=V1k1V9Ra1FoC&pg=PA741
    const auto sqrRadius = wwww(sphere.centerRadius * sphere.centerRadius);
    const auto originToCenter = zeroW(sphere.centerRadius - ray.origin);
    const auto s = dotv(originToCenter, ray.direction);
    const auto sqrLengthOriginToCenter = dotv(originToCenter, originToCenter);
    const auto sqrM = sqrLengthOriginToCenter - s * s;

    if (const auto sphereIsBeforeAndOutOfReach = isNegative(s) & x(sqrLengthOriginToCenter > sqrRadius);
        sphereIsBeforeAndOutOfReach | x(sqrM > sqrRadius)) {

      return NegativeInfinity<Float4>();
    }

    const auto q = sqrt(sqrRadius - sqrM);
    // d.x = d.z = s - q;
    // d.y = d.w = s + q;
    const auto d = subadd(s, q);
    return blend<false, true, false, true>(min(d, yyww(d)), max(xxzz(d), d));
  }

  inline Float4::VectorBoolType testSphereIntersectionCoefficients(const Float4 &c) noexcept {
    // x = t0 < 0 = c.x < 0
    // y = t1 < 0 = c.y < 0
    // z = t0 < 0 = c.z < 0
    // w = t1 < 0 = c.w < 0
    return c < Zero<Float4>();
  }

  bool overlaps(const RayCast &rayCast, const BoundingSphere &sphere) noexcept {
    const auto coefficients = computeSphereIntersectionCoefficients(rayCast.ray, sphere);
    const auto check = testSphereIntersectionCoefficients(coefficients);
    // overlaps if coefficients.y >= 0 and (coefficients.x < 0 or ray reaches front of the sphere)
    return !!x(andnot(andnot(check, outOfReach(rayCast, coefficients)), !yyyy(check)));
  }

  Float
  nearestIntersection(const RayCast &rayCast, const BoundingSphere &sphere, const Size2::ValueType originId) noexcept {
    const auto coefficients = computeSphereIntersectionCoefficients(rayCast.ray, sphere);

    // rayCast starts behind the sphere
    if (const auto check = testSphereIntersectionCoefficients(coefficients); y(check)) {
      return rayCast.maxDistance;
    }

    // reject t.y if backface culled
    const auto ty = yyww(coefficients);
    const auto selfOccludedY = convert<Float4::VectorBoolType>(selfOcclusion(rayCast, originId, x(ty)));
    const auto tt = blendMasked(ty, Float4(rayCast.maxDistance), selfOccludedY | backfaceCulledv(rayCast));

    // reject t.x if frontfaceCulled or overlaps or selfOccluded
    const auto selfOccludedX = convert<Float4::VectorBoolType>(selfOcclusion(rayCast, originId, x(coefficients)));
    return x(blendMasked(coefficients, tt, selfOccludedX | frontfaceCulledv(rayCast)));
  }
}
