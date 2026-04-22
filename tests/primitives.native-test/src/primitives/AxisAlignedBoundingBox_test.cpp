#include "primitives_tests.h"

namespace primitives::test {
  TEST_CLASS(AxisAlignedBoundingBoxTest) {
  public:
    TEST_METHOD(constructsDefault) {
      const AxisAlignedBoundingBox box{};
      Assert::AreEqual(std::numeric_limits<float>::max(), x(box.minimum), L"minimum x should be FLT_MAX", LINE_INFO());
      Assert::AreEqual(std::numeric_limits<float>::max(), y(box.minimum), L"minimum y should be FLT_MAX", LINE_INFO());
      Assert::AreEqual(std::numeric_limits<float>::max(), z(box.minimum), L"minimum z should be FLT_MAX", LINE_INFO());
      Assert::AreEqual(
          std::numeric_limits<float>::lowest(), x(box.maximum), L"maximum x should be FLT_LOWEST", LINE_INFO()
      );
      Assert::AreEqual(
          std::numeric_limits<float>::lowest(), y(box.maximum), L"maximum y should be FLT_LOWEST", LINE_INFO()
      );
      Assert::AreEqual(
          std::numeric_limits<float>::lowest(), z(box.maximum), L"maximum z should be FLT_LOWEST", LINE_INFO()
      );
    }

    TEST_METHOD(constructsFromTwoPoints) {
      const Float4 a{3.0f, 1.0f, 2.0f, 0.0f};
      const Float4 b{1.0f, 3.0f, 0.0f, 0.0f};
      const AxisAlignedBoundingBox box{a, b};
      const Float4 expectedMin{1.0f, 1.0f, 0.0f, 0.0f};
      const Float4 expectedMax{3.0f, 3.0f, 2.0f, 0.0f};
      Assert::IsTrue(allTrue(box.minimum == expectedMin), L"minimum should be component-wise min", LINE_INFO());
      Assert::IsTrue(allTrue(box.maximum == expectedMax), L"maximum should be component-wise max", LINE_INFO());
    }

    TEST_METHOD(computesExtents) {
      const AxisAlignedBoundingBox box{Float4{0.0f, 0.0f, 0.0f, 0.0f}, Float4{2.0f, 4.0f, 6.0f, 0.0f}};
      const Float4 expected{2.0f, 4.0f, 6.0f, 0.0f};
      Assert::IsTrue(allTrue(extents(box) == expected), L"extents should be max - min", LINE_INFO());
    }

    TEST_METHOD(computesHalfLengths) {
      const AxisAlignedBoundingBox box{Float4{0.0f, 0.0f, 0.0f, 0.0f}, Float4{2.0f, 4.0f, 6.0f, 0.0f}};
      const Float4 expected{1.0f, 2.0f, 3.0f, 0.0f};
      Assert::IsTrue(allTrue(halfLengths(box) == expected), L"halfLengths should be extents / 2", LINE_INFO());
    }

    TEST_METHOD(computesCenter) {
      const AxisAlignedBoundingBox box{Float4{0.0f, 0.0f, 0.0f, 0.0f}, Float4{2.0f, 4.0f, 6.0f, 0.0f}};
      const Float4 expected{1.0f, 2.0f, 3.0f, 0.0f};
      Assert::IsTrue(allTrue(center(box) == expected), L"center should be halfLengths + min", LINE_INFO());
    }

    TEST_METHOD(computesSurfaceArea) {
      const AxisAlignedBoundingBox box{Float4{0.0f, 0.0f, 0.0f, 0.0f}, Float4{2.0f, 3.0f, 4.0f, 0.0f}};
      // 2*(2*3 + 2*4 + 3*4) = 2*26 = 52
      Assert::AreEqual(52.0f, surfaceArea(box), L"surfaceArea should be 2*(xy + xz + yz)", LINE_INFO());
    }

    TEST_METHOD(extendsByPoint) {
      const AxisAlignedBoundingBox box{Float4{0.0f, 0.0f, 0.0f, 0.0f}, Float4{1.0f, 1.0f, 1.0f, 0.0f}};
      const Float4 point{-1.0f, 2.0f, 0.0f, 0.0f};
      const auto extended = extendBy(box, point);
      Assert::AreEqual(-1.0f, x(extended.minimum), L"minimum x should be extended to -1", LINE_INFO());
      Assert::AreEqual(0.0f, y(extended.minimum), L"minimum y should remain 0", LINE_INFO());
      Assert::AreEqual(0.0f, z(extended.minimum), L"minimum z should remain 0", LINE_INFO());
      Assert::AreEqual(1.0f, x(extended.maximum), L"maximum x should remain 1", LINE_INFO());
      Assert::AreEqual(2.0f, y(extended.maximum), L"maximum y should be extended to 2", LINE_INFO());
      Assert::AreEqual(1.0f, z(extended.maximum), L"maximum z should remain 1", LINE_INFO());
    }

    TEST_METHOD(extendsByBox) {
      const AxisAlignedBoundingBox a{Float4{0.0f, 0.0f, 0.0f, 0.0f}, Float4{1.0f, 1.0f, 1.0f, 0.0f}};
      const AxisAlignedBoundingBox b{Float4{-1.0f, -1.0f, -1.0f, 0.0f}, Float4{0.5f, 0.5f, 0.5f, 0.0f}};
      const auto united = extendBy(a, b);
      const Float4 expectedMin{-1.0f, -1.0f, -1.0f, 0.0f};
      const Float4 expectedMax{1.0f, 1.0f, 1.0f, 0.0f};
      Assert::IsTrue(allTrue(united.minimum == expectedMin), L"union minimum should be component-wise min", LINE_INFO());
      Assert::IsTrue(allTrue(united.maximum == expectedMax), L"union maximum should be component-wise max", LINE_INFO());
    }

    TEST_METHOD(clampsToOther) {
      const AxisAlignedBoundingBox a{Float4{-2.0f, -2.0f, -2.0f, 0.0f}, Float4{2.0f, 2.0f, 2.0f, 0.0f}};
      const AxisAlignedBoundingBox b{Float4{-1.0f, -1.0f, -1.0f, 0.0f}, Float4{1.0f, 1.0f, 1.0f, 0.0f}};
      const auto clamped = clamp(a, b);
      const Float4 expectedMin{-1.0f, -1.0f, -1.0f, 0.0f};
      const Float4 expectedMax{1.0f, 1.0f, 1.0f, 0.0f};
      Assert::IsTrue(
          allTrue(clamped.minimum == expectedMin), L"clamped minimum should match clamp box min", LINE_INFO()
      );
      Assert::IsTrue(
          allTrue(clamped.maximum == expectedMax), L"clamped maximum should match clamp box max", LINE_INFO()
      );
    }

    TEST_METHOD(overlapsPoint) {
      // overlaps(box, point) returns true when min >= point AND max <= point for xyz,
      // i.e. the box is "contained within" the point (degenerate check).
      // A point equal to both min and max of a zero-extent box satisfies this.
      const AxisAlignedBoundingBox zeroBox{Float4{1.0f, 1.0f, 1.0f, 0.0f}, Float4{1.0f, 1.0f, 1.0f, 0.0f}};
      Assert::IsTrue(overlaps(zeroBox, Float4{1.0f, 1.0f, 1.0f, 0.0f}), L"point at zero-extent box should overlap", LINE_INFO());
      Assert::IsFalse(overlaps(zeroBox, Float4{0.0f, 0.0f, 0.0f, 0.0f}), L"different point should not overlap", LINE_INFO());
    }

    TEST_METHOD(overlapsOtherBox) {
      const AxisAlignedBoundingBox a{Float4{0.0f, 0.0f, 0.0f, 0.0f}, Float4{2.0f, 2.0f, 2.0f, 0.0f}};
      const AxisAlignedBoundingBox b{Float4{1.0f, 1.0f, 1.0f, 0.0f}, Float4{3.0f, 3.0f, 3.0f, 0.0f}};
      const AxisAlignedBoundingBox c{Float4{5.0f, 5.0f, 5.0f, 0.0f}, Float4{6.0f, 6.0f, 6.0f, 0.0f}};
      Assert::IsTrue(overlaps(a, b), L"overlapping boxes should return true", LINE_INFO());
      Assert::IsFalse(overlaps(a, c), L"disjoint boxes should return false", LINE_INFO());
    }

    TEST_METHOD(overlapsRayHit) {
      const Ray ray{Float4{0.0f, 0.0f, -5.0f, 0.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      const AxisAlignedBoundingBox box{Float4{-1.0f, -1.0f, -1.0f, 0.0f}, Float4{1.0f, 1.0f, 1.0f, 0.0f}};
      Assert::IsTrue(overlaps(rayCast, box), L"ray aimed at box should overlap", LINE_INFO());
    }

    TEST_METHOD(overlapsRayMiss) {
      const Ray ray{Float4{0.0f, 0.0f, -5.0f, 0.0f}, Float4{0.0f, 1.0f, 0.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      const AxisAlignedBoundingBox box{Float4{-1.0f, -1.0f, -1.0f, 0.0f}, Float4{1.0f, 1.0f, 1.0f, 0.0f}};
      Assert::IsFalse(overlaps(rayCast, box), L"ray aimed away should not overlap", LINE_INFO());
    }

    TEST_METHOD(computesNearestIntersectionHit) {
      const Ray ray{Float4{0.0f, 0.0f, -5.0f, 0.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 1), 100.0f};
      const AxisAlignedBoundingBox box{Float4{-1.0f, -1.0f, -1.0f, 0.0f}, Float4{1.0f, 1.0f, 1.0f, 0.0f}};
      const Float distance = nearestIntersection(rayCast, box, Size2::ValueType(0));
      Assert::AreEqual(4.0f, distance, L"nearest intersection should be at distance 4", LINE_INFO());
    }

    TEST_METHOD(computesNearestIntersectionMiss) {
      const Ray ray{Float4{0.0f, 0.0f, -5.0f, 0.0f}, Float4{0.0f, 1.0f, 0.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      const AxisAlignedBoundingBox box{Float4{-1.0f, -1.0f, -1.0f, 0.0f}, Float4{1.0f, 1.0f, 1.0f, 0.0f}};
      const Float distance = nearestIntersection(rayCast, box, Size2::ValueType(0));
      Assert::AreEqual(100.0f, distance, L"missed intersection should return maxDistance", LINE_INFO());
    }
  };
}
