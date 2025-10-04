#include "primitives/AxisAlignedBoundingBox.h"

#include <limits>

using namespace vectorization;

namespace primitives {
  AxisAlignedBoundingBox::AxisAlignedBoundingBox() noexcept
      : minimum(std::numeric_limits<Float4::ValueType>::max()),
        maximum(std::numeric_limits<Float4::ValueType>::lowest()) {
  }

  AxisAlignedBoundingBox::AxisAlignedBoundingBox(const Float4 &a, const Float4 &b) noexcept
      : minimum(min(a, b)), maximum(max(a, b)) {
  }

  Float4 extents(const AxisAlignedBoundingBox &box) noexcept {
    return box.maximum - box.minimum;
  }

  Float4 halfLengths(const AxisAlignedBoundingBox &box) noexcept {
    return extents(box) * Half<Float4>();
  }

  Float4 center(const AxisAlignedBoundingBox &box) noexcept {
    return center(box, halfLengths(box));
  }

  Float4 center(const AxisAlignedBoundingBox &box, const Float4 &halfLengths) noexcept {
    return halfLengths + box.minimum;
  }

  Float surfaceArea(const AxisAlignedBoundingBox &box) noexcept {
    return surfaceArea(box, extents(box));
  }

  Float surfaceArea(const AxisAlignedBoundingBox & /* box */, const Float4 &extents) noexcept {
    const auto t = zzyy(extents) * yxxy(extents);
    return horizontalSum3(t + t);
  }

  AxisAlignedBoundingBox extendBy(const AxisAlignedBoundingBox &box, const Float4 &by) noexcept {
    AxisAlignedBoundingBox r;
    r.minimum = min(by, box.minimum);
    r.maximum = max(by, box.maximum);
    return r;
  }

  AxisAlignedBoundingBox
  extendBy(const AxisAlignedBoundingBox &box, const Float4 &byMin, const Float4 &byMax) noexcept {
    AxisAlignedBoundingBox r;
    r.minimum = min(box.minimum, byMin);
    r.maximum = max(box.maximum, byMax);
    return r;
  }

  AxisAlignedBoundingBox extendBy(const AxisAlignedBoundingBox &box, const AxisAlignedBoundingBox &by) noexcept {
    return extendBy(box, by.minimum, by.maximum);
  }

  AxisAlignedBoundingBox clamp(const AxisAlignedBoundingBox &box, const AxisAlignedBoundingBox &by) noexcept {
    AxisAlignedBoundingBox r;
    r.minimum = clamp(box.minimum, by.minimum, by.maximum);
    r.maximum = clamp(box.maximum, by.minimum, by.maximum);
    return r;
  }

  template <ASizeT RowIndex>
  struct TransformationHelper {
    static void
    apply(AxisAlignedBoundingBox &out, const AxisAlignedBoundingBox &toTransform, const Float44 &m) noexcept {
      auto f = row<RowIndex>(m);
      auto a = f * toTransform.minimum, b = f * toTransform.maximum;
      out.minimum =
          replaceComponent<RowIndex>(out.minimum, component<RowIndex>(out.minimum + horizontalSum3v(min(a, b))));
      out.maximum =
          replaceComponent<RowIndex>(out.maximum, component<RowIndex>(out.maximum + horizontalSum3v(max(a, b))));
    }
  };

  AxisAlignedBoundingBox transform(const AxisAlignedBoundingBox &box, const Float44 &by) noexcept {
    // Take care of translation first
    const auto t = yw_yw(
        zw_zw(row<VectorIndices::X>(by), row<VectorIndices::Y>(by)), zw_zw(row<VectorIndices::Z>(by), One<Float4>())
    );

    AxisAlignedBoundingBox boxOut;
    boxOut.minimum = t;
    boxOut.maximum = t;
    // Now find the extreme points by considering the product of the
    // min and max with each row of by.
    TransformationHelper<VectorIndices::X>::apply(boxOut, box, by);
    TransformationHelper<VectorIndices::Y>::apply(boxOut, box, by);
    TransformationHelper<VectorIndices::Z>::apply(boxOut, box, by);
    return boxOut;
  }

  bool overlaps(const AxisAlignedBoundingBox &box, const Float4 &by) noexcept {
    const auto minimumIsBefore = box.minimum >= by;
    const auto maximumIsAfter = box.maximum <= by;
    return allTrue3(minimumIsBefore & maximumIsAfter);
  }

  bool overlaps(const AxisAlignedBoundingBox &a, const AxisAlignedBoundingBox &b) noexcept {
    const auto minimumAIsBeforeMaximumB = a.minimum > b.maximum;
    const auto minimumBIsBeforeMaximumA = b.minimum > a.maximum;
    return allFalse3(minimumAIsBeforeMaximumB | minimumBIsBeforeMaximumA);
  }

  /**
   * @see Based on: "A Cross-Platform Framework for Interactive Ray Tracing", Geimer and Müller.
   * @param ray ray
   * @param a first box to test
   * @param b second box to test
   * @return .x: near distance to "a", .y: far distance to "a", .z: near distance to "b", .w: far distance to "b"
   */
  inline Float4 computeBoxIntersectionCoefficients(
      const Ray &ray, const AxisAlignedBoundingBox &a, const AxisAlignedBoundingBox &b
  ) noexcept {
    auto pMinA = (a.minimum - ray.origin) * ray.reciprocalDirection;
    auto pMaxA = (a.maximum - ray.origin) * ray.reciprocalDirection;
    auto pMinB = (b.minimum - ray.origin) * ray.reciprocalDirection;
    auto pMaxB = (b.maximum - ray.origin) * ray.reciprocalDirection;

    auto vMinA = min(pMinA, pMaxA);
    auto vMaxA = max(pMinA, pMaxA);
    auto vMinB = min(pMinB, pMaxB);
    auto vMaxB = max(pMinB, pMaxB);

    auto hA = xy_xy(vMinA, vMaxA);
    auto hB = xy_xy(vMinB, vMaxB);
    auto hX = xz_xz(hA, hB);
    auto hY = yw_yw(hA, hB);
    auto hZ = xz_xz(zw_zw(vMinA, vMaxA), zw_zw(vMinB, vMaxB));

    return blend<false, true, false, true>(max(hZ, max(hX, hY)), min(hZ, min(hX, hY)));
  }

  Float4::VectorBoolType
  overlaps(const RayCast &rayCast, const AxisAlignedBoundingBox &a, const AxisAlignedBoundingBox &b) noexcept {
    const auto distances = computeBoxIntersectionCoefficients(rayCast.ray, a, b);
    const auto farDistances = yyww(distances);
    const auto nearDistances = xxzz(distances);
    // overlaps if
    // 1. (c.y >= c.x, c.w >= c.z) and
    // 2. (c.y >= 0, c.w >= 0) and
    // 3. (c.x < 0, c.z < 0) or ray is in front and reaches the boxes
    // optimized with c & (a | !b) == !(!a & b) & c using andnot(x, y) = !x & y
    const auto validDistances = farDistances >= nearDistances;
    const auto positiveFarDistances = farDistances >= Zero<Float4>();
    const auto negativeNearDistances = nearDistances < Zero<Float4>();
    return validDistances
           & andnot(andnot(negativeNearDistances, outOfReach(rayCast, nearDistances)), positiveFarDistances);
  }

  // The near distance t0 is set to "t.x". The far distance t1 is set "t.y" while "t.z" and "t.w" will be set to zero.
  inline Float4 computeBoxIntersectionCoefficients(const Ray &ray, const AxisAlignedBoundingBox &box) noexcept {
    // based on: "A Cross-Platform Framework for Interactive Ray Tracing", Geimer and Müller.
    const auto pMin = (box.minimum - ray.origin) * ray.reciprocalDirection;
    const auto pMax = (box.maximum - ray.origin) * ray.reciprocalDirection;
    const auto vMin = min(pMin, pMax);
    const auto vMax = max(pMin, pMax);
    const auto vXYx2 = xy_xy(vMin, vMax);
    const auto vXZx2 = xz_xz(vMin, vMax);
    const auto hMinXXYZx2 = min(vXYx2, vXZx2);
    const auto hMaxXXYZx2 = max(vXYx2, vXZx2);
    // x = t0, y = t1, z = w = 0
    return xz_xz(xy_zw(max(hMaxXXYZx2, yyww(hMaxXXYZx2)), min(hMinXXYZx2, yyww(hMinXXYZx2))), Zero<Float4>());
  }

  /**
   * Compares box intersection near distance and far distance
   * @param c .x: near distance, .y: far distance, .z: 0
   * @return .x: c.x \< c.x = always false, .y: c.y \< c.x, .z: c.x \< c.z, .w: c.y \< c.z
   */
  inline Float4::VectorBoolType testBoxIntersectionCoefficients(const Float4 &c) noexcept {
    return xyxy(c) < xxzz(c);
  }

  bool overlaps(const RayCast &rayCast, const AxisAlignedBoundingBox &box) noexcept {
    const auto c = computeBoxIntersectionCoefficients(rayCast.ray, box);
    const auto check = testBoxIntersectionCoefficients(c);

    // no hit when t.y < t.x or t.y < 0
    if (y(check | wwww(check))) {
      return false;
    }

    // if check.z is true, then ray overlaps the box; otherwise ray is in front and may not reach it
    // optimized with (box | !b) == !(!box & b) using andnot(x, y) = !x & y
    return !x(andnot(zzww(check), outOfReach(rayCast, c)));
  }

  Float nearestIntersection(
      const RayCast &rayCast, const AxisAlignedBoundingBox &box, const Size2::ValueType originId
  ) noexcept {
    const auto c = computeBoxIntersectionCoefficients(rayCast.ray, box);
    const auto check = testBoxIntersectionCoefficients(c);

    // no hit when c.y < c.x or c.y < 0
    if (y(check | wwww(check))) {
      return rayCast.maxDistance;
    }

    // reject c.y if backface culled
    const auto tt = blendMasked(yyww(c), Float4(rayCast.maxDistance), backfaceCulledv(rayCast));

    // reject c.x if c.x < 0 or selfOccluded or frontfaceCulled
    const auto selfOccluded = convert<Float4::VectorBoolType>(selfOcclusion(rayCast, originId));
    return x(blendMasked(c, tt, zwzw(check) | selfOccluded | frontfaceCulledv(rayCast)));
  }
}
