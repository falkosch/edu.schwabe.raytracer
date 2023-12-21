#include "primitives/BoundingSphere.h"

using namespace vectorization;

namespace primitives {
  BoundingSphere::BoundingSphere() noexcept : centerRadius(OneW<Float4>()) {
  }

  BoundingSphere::BoundingSphere(const Float4 &center, const Float radius) noexcept
      : centerRadius(replaceW(center, radius)) {
  }

  const AxisAlignedBoundingBox bounding(const BoundingSphere &b) noexcept {
    auto radius = wwww(b.centerRadius);
    return AxisAlignedBoundingBox(oneW(b.centerRadius - radius), oneW(b.centerRadius + radius));
  }

  inline const Float4 computeSphereIntersectionCoefficients(const Ray &r, const BoundingSphere &b) noexcept {
    // geometric method from Real-Time Rendering 3: http://books.google.de/books?id=V1k1V9Ra1FoC&pg=PA741
    auto sqrRadius = wwww(b.centerRadius * b.centerRadius);
    auto vl = zeroW(b.centerRadius - r.origin);
    auto s = dotv(vl, r.direction);
    auto ll = dotv(vl, vl);
    auto sqrM = ll - s * s;
    if ((isNegative(s) & x(ll > sqrRadius)) | x(sqrM > sqrRadius)) {
      return NegativeInfinity<Float4>();
    }

    auto q = sqrt(sqrRadius - sqrM);
    // d.x = d.z = s - q;
    // d.y = d.w = s + q;
    auto d = subadd(s, q);
    return blend<false, true, false, true>(min(d, yyww(d)), max(xxzz(d), d));
  }

  inline const Float4::VectorBoolType testSphereIntersectionCoefficients(const Float4 &c) noexcept {
    // x = t0 < 0 = c.x < 0
    // y = t1 < 0 = c.y < 0
    // z = t0 < 0 = c.z < 0
    // w = t1 < 0 = c.w < 0
    return c < Zero<Float4>();
  }

  const bool overlaps(const RayCast &r, const BoundingSphere &b) noexcept {
    auto coefficients = computeSphereIntersectionCoefficients(r.ray, b);
    auto check = testSphereIntersectionCoefficients(coefficients);
    // overlaps if coefficients.y >= 0 and (coefficients.x < 0 or ray reaches front of sphere)
    return !!x(andnot(andnot(check, outOfReach(r, coefficients)), !yyyy(check)));
  }

  const Float
  nearestIntersection(const RayCast &rayCast, const BoundingSphere &b, const Size2::ValueType originId) noexcept {
    auto coefficients = computeSphereIntersectionCoefficients(rayCast.ray, b);
    auto check = testSphereIntersectionCoefficients(coefficients);

    // rayCast starts behind sphere
    if (y(check)) {
      return rayCast.maxDistance;
    }

    // reject t.y if backface culled
    auto ty = yyww(coefficients);
    auto selfOccludedY = convert<Float4::VectorBoolType>(selfOcclusion(rayCast, originId, x(ty)));
    auto tt = blendMasked(ty, Float4(rayCast.maxDistance), selfOccludedY | backfaceCulledv(rayCast));

    // reject t.x if frontfaceCulled or overlaps or selfOccluded
    auto selfOccludedX = convert<Float4::VectorBoolType>(selfOcclusion(rayCast, originId, x(coefficients)));
    return x(blendMasked(coefficients, tt, selfOccludedX | frontfaceCulledv(rayCast)));
  }
}
