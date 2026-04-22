#include "primitives_tests.h"

namespace primitives::test {
  TEST_CLASS(SplittingPlaneTest) {
  public:
    TEST_METHOD(constructsDefault) {
      const SplittingPlane plane{};
      Assert::IsTrue(
          allTrue(plane.normalDistance == OneZ<Float4>()), L"default normalDistance should be (0,0,1,0)", LINE_INFO()
      );
    }

    TEST_METHOD(constructsFromNormalDistance) {
      const Float4 nd{0.0f, 1.0f, 0.0f, 5.0f};
      const SplittingPlane plane{nd};
      Assert::IsTrue(allTrue(plane.normalDistance == nd), L"normalDistance should be stored as-is", LINE_INFO());
    }

    TEST_METHOD(constructsFromOriginAndNormal) {
      const Float4 origin{0.0f, 0.0f, 5.0f, 0.0f};
      const Float4 normal{0.0f, 0.0f, 1.0f, 0.0f};
      const SplittingPlane plane{origin, normal};
      Assert::AreEqual(0.0f, x(plane.normalDistance), L"normal x should be 0", LINE_INFO());
      Assert::AreEqual(0.0f, y(plane.normalDistance), L"normal y should be 0", LINE_INFO());
      Assert::AreEqual(1.0f, z(plane.normalDistance), L"normal z should be 1", LINE_INFO());
      Assert::AreEqual(5.0f, w(plane.normalDistance), L"distance should be 5", LINE_INFO());
    }

    TEST_METHOD(overlapsBoxTrue) {
      const SplittingPlane plane{Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const AxisAlignedBoundingBox box{
          Float4{-1.0f, -1.0f, -1.0f, 0.0f}, Float4{1.0f, 1.0f, 1.0f, 0.0f}
      };
      Assert::IsTrue(overlaps(box, plane), L"box straddling plane should overlap", LINE_INFO());
    }

    TEST_METHOD(overlapsBoxFalse) {
      const SplittingPlane plane{Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const AxisAlignedBoundingBox box{
          Float4{2.0f, 2.0f, 2.0f, 0.0f}, Float4{3.0f, 3.0f, 3.0f, 0.0f}
      };
      Assert::IsFalse(overlaps(box, plane), L"box entirely on one side should not overlap", LINE_INFO());
    }

    TEST_METHOD(overlapsRayHit) {
      const SplittingPlane plane{Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const Float4 origin{0.0f, 0.0f, -5.0f, 0.0f};
      const Float4 direction{0.0f, 0.0f, 1.0f, 0.0f};
      const Ray ray{origin, direction};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      Assert::IsTrue(overlaps(rayCast, plane), L"ray aimed at plane should overlap", LINE_INFO());
    }

    TEST_METHOD(overlapsRayMiss) {
      const SplittingPlane plane{Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const Float4 origin{0.0f, 0.0f, -5.0f, 0.0f};
      const Float4 direction{1.0f, 0.0f, 0.0f, 0.0f};
      const Ray ray{origin, direction};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      Assert::IsFalse(overlaps(rayCast, plane), L"ray parallel to plane should not overlap", LINE_INFO());
    }

    TEST_METHOD(computesNearestIntersectionHit) {
      const SplittingPlane plane{Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const Float4 origin{0.0f, 0.0f, -5.0f, 0.0f};
      const Float4 direction{0.0f, 0.0f, 1.0f, 0.0f};
      const Ray ray{origin, direction};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      const auto t = nearestIntersection(rayCast, plane, 0);
      Assert::AreEqual(5.0f, t, 0.001f, L"distance should be 5.0", LINE_INFO());
    }

    TEST_METHOD(computesNearestIntersectionMiss) {
      // Ray aimed away from plane — should miss
      const SplittingPlane plane{Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const Float4 origin{0.0f, 0.0f, -5.0f, 0.0f};
      const Float4 direction{0.0f, 1.0f, 0.0f, 0.0f};
      const Ray ray{origin, direction};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 1), 100.0f};
      const auto t = nearestIntersection(rayCast, plane, 0);
      Assert::IsTrue(t >= rayCast.maxDistance, L"parallel ray should not produce nearer intersection", LINE_INFO());
    }
  };
}
