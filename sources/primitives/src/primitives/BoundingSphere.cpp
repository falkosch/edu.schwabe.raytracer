#include "primitives/BoundingSphere.h"

using namespace vectorization;

namespace primitives {
  BoundingSphere::BoundingSphere() noexcept : centerRadius(OneW<Float4>()) {
  }

  BoundingSphere::BoundingSphere(const Float4 &center, const Float radius) noexcept
      : centerRadius(replaceW(center, radius)) {
  }

  AxisAlignedBoundingBox bounding(const BoundingSphere &b) noexcept {
    const auto radius = wwww(b.centerRadius);
    return AxisAlignedBoundingBox(oneW(b.centerRadius - radius), oneW(b.centerRadius + radius));
  }

  inline Float4 computeSphereIntersectionCoefficients(const Ray &r, const BoundingSphere &b) noexcept {
    // geometric method from Real-Time Rendering 3: http://books.google.de/books?id=V1k1V9Ra1FoC&pg=PA741
    const auto sqrRadius = wwww(b.centerRadius * b.centerRadius);
    const auto vl = zeroW(b.centerRadius - r.origin);
    const auto s = dotv(vl, r.direction);
    const auto ll = dotv(vl, vl);
    const auto sqrM = ll - s * s;
    if ((isNegative(s) & x(ll > sqrRadius)) | x(sqrM > sqrRadius)) {
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

  bool overlaps(const RayCast &rayCast, const BoundingSphere &by) noexcept {
    const auto coefficients = computeSphereIntersectionCoefficients(rayCast.ray, by);
    const auto check = testSphereIntersectionCoefficients(coefficients);
    // overlaps if coefficients.y >= 0 and (coefficients.x < 0 or ray reaches front of sphere)
    return !!x(andnot(andnot(check, outOfReach(rayCast, coefficients)), !yyyy(check)));
  }

  Float nearestIntersection(const RayCast &rayCast, const BoundingSphere &b, const Size2::ValueType originId) noexcept {
    const auto coefficients = computeSphereIntersectionCoefficients(rayCast.ray, b);
    const auto check = testSphereIntersectionCoefficients(coefficients);

    // rayCast starts behind sphere
    if (y(check)) {
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
