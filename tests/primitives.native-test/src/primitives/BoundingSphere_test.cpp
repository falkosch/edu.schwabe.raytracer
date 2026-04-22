#include "primitives_tests.h"

namespace primitives::test {
  TEST_CLASS(BoundingSphereTest) {
  public:
    TEST_METHOD(constructsDefault) {
      const BoundingSphere sphere{};
      Assert::IsTrue(
          allTrue(sphere.centerRadius == OneW<Float4>()), L"default centerRadius should be (0,0,0,1)", LINE_INFO()
      );
    }

    TEST_METHOD(constructsFromCenterAndRadius) {
      const Float4 center{1.0f, 2.0f, 3.0f, 0.0f};
      const BoundingSphere sphere{center, 5.0f};
      Assert::AreEqual(1.0f, x(sphere.centerRadius), L"center x should be 1", LINE_INFO());
      Assert::AreEqual(2.0f, y(sphere.centerRadius), L"center y should be 2", LINE_INFO());
      Assert::AreEqual(3.0f, z(sphere.centerRadius), L"center z should be 3", LINE_INFO());
      Assert::AreEqual(5.0f, w(sphere.centerRadius), L"radius should be 5", LINE_INFO());
    }

    TEST_METHOD(computesBoundingBox) {
      const BoundingSphere sphere{Float4{0.0f, 0.0f, 0.0f, 0.0f}, 2.0f};
      const auto box = bounding(sphere);
      Assert::AreEqual(-2.0f, x(box.minimum), L"bounding min x should be -2", LINE_INFO());
      Assert::AreEqual(-2.0f, y(box.minimum), L"bounding min y should be -2", LINE_INFO());
      Assert::AreEqual(-2.0f, z(box.minimum), L"bounding min z should be -2", LINE_INFO());
      Assert::AreEqual(2.0f, x(box.maximum), L"bounding max x should be 2", LINE_INFO());
      Assert::AreEqual(2.0f, y(box.maximum), L"bounding max y should be 2", LINE_INFO());
      Assert::AreEqual(2.0f, z(box.maximum), L"bounding max z should be 2", LINE_INFO());
    }

    TEST_METHOD(overlapsRayHit) {
      const Ray ray{Float4{0.0f, 0.0f, -5.0f, 0.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      const BoundingSphere sphere{Float4{0.0f, 0.0f, 0.0f, 0.0f}, 1.0f};
      Assert::IsTrue(overlaps(rayCast, sphere), L"ray aimed at sphere should overlap", LINE_INFO());
    }

    TEST_METHOD(overlapsRayMiss) {
      const Ray ray{Float4{0.0f, 0.0f, -5.0f, 0.0f}, Float4{0.0f, 1.0f, 0.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      const BoundingSphere sphere{Float4{0.0f, 0.0f, 0.0f, 0.0f}, 1.0f};
      Assert::IsFalse(overlaps(rayCast, sphere), L"ray aimed away should not overlap", LINE_INFO());
    }

    TEST_METHOD(computesNearestIntersectionHit) {
      const Ray ray{Float4{0.0f, 0.0f, -5.0f, 0.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      const BoundingSphere sphere{Float4{0.0f, 0.0f, 0.0f, 0.0f}, 1.0f};
      const Float distance = nearestIntersection(rayCast, sphere, Size2::ValueType(0));
      Assert::AreEqual(4.0f, distance, L"nearest intersection should be at distance 4", LINE_INFO());
    }

    TEST_METHOD(computesNearestIntersectionMiss) {
      const Ray ray{Float4{0.0f, 0.0f, -5.0f, 0.0f}, Float4{0.0f, 1.0f, 0.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      const BoundingSphere sphere{Float4{0.0f, 0.0f, 0.0f, 0.0f}, 1.0f};
      const Float distance = nearestIntersection(rayCast, sphere, Size2::ValueType(0));
      Assert::AreEqual(100.0f, distance, L"missed intersection should return maxDistance", LINE_INFO());
    }
  };
}
