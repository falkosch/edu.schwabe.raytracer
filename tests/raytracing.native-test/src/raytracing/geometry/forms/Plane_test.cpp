#include "raytracing_tests.h"

namespace raytracer::test {
  TEST_CLASS(PlaneTest) {
  public:
    TEST_METHOD(constructsDefault) {
      const Plane plane{};
      Assert::IsTrue(plane.isInfinite(), L"plane should be infinite", LINE_INFO());
    }

    TEST_METHOD(constructsFromOriginAndNormal) {
      const Plane plane{Float4{0.0f, 0.0f, 0.0f, 0.0f}, Float4{0.0f, 1.0f, 0.0f, 0.0f}};
      Assert::IsTrue(plane.isInfinite(), L"should be infinite", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionHitsDefaultPlane) {
      const Plane plane{};
      const Ray ray{Float4{0.0f, 0.0f, -5.0f, 0.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = plane.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(5.0f, distance, L"should hit z=0 plane at distance 5", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionHitsFromNegativeZ) {
      const Plane plane{};
      const Ray ray{Float4{3.0f, 2.0f, -10.0f, 0.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = plane.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(10.0f, distance, L"should hit z=0 at distance 10", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionMissesParallelRay) {
      const Plane plane{};
      const Ray ray{Float4{0.0f, 0.0f, -5.0f, 0.0f}, Float4{1.0f, 0.0f, 0.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = plane.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(100.0f, distance, L"parallel ray should miss", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionMissesRayAwayFromPlane) {
      const Plane plane{};
      const Ray ray{Float4{0.0f, 0.0f, -5.0f, 0.0f}, Float4{0.0f, 0.0f, -1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = plane.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(100.0f, distance, L"ray going away should miss", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionBeyondMaxDistance) {
      const Plane plane{};
      const Ray ray{Float4{0.0f, 0.0f, -5.0f, 0.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 3.0f};
      FacetIntersection intersection{};

      const auto distance = plane.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(3.0f, distance, L"hit beyond maxDistance should return maxDistance", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionSetsNormal) {
      const Plane plane{};
      const Ray ray{Float4{0.0f, 0.0f, -5.0f, 0.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      plane.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(0.0f, x(intersection.surfaceNormal), L"normal x", LINE_INFO());
      Assert::AreEqual(0.0f, y(intersection.surfaceNormal), L"normal y", LINE_INFO());
      Assert::AreEqual(1.0f, z(intersection.surfaceNormal), L"normal z should be +z", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionSetsVertex) {
      const Plane plane{};
      const Ray ray{Float4{2.0f, 3.0f, -5.0f, 0.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      plane.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(2.0f, x(intersection.vertex), L"vertex x should match ray origin x", LINE_INFO());
      Assert::AreEqual(3.0f, y(intersection.vertex), L"vertex y should match ray origin y", LINE_INFO());
      Assert::AreEqual(0.0f, z(intersection.vertex), L"vertex z should be on z=0 plane", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionSetsNode) {
      const Plane plane{};
      const Ray ray{Float4{0.0f, 0.0f, -5.0f, 0.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      plane.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::IsTrue(intersection.node == &plane, L"node should point to plane", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionYPlane) {
      const Plane plane{Float4{0.0f, 0.0f, 0.0f, 0.0f}, Float4{0.0f, 1.0f, 0.0f, 0.0f}};
      const Ray ray{Float4{0.0f, -5.0f, 0.0f, 0.0f}, Float4{0.0f, 1.0f, 0.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = plane.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(5.0f, distance, L"should hit y=0 plane at distance 5", LINE_INFO());
      Assert::AreEqual(0.0f, x(intersection.surfaceNormal), L"normal x", LINE_INFO());
      Assert::AreEqual(1.0f, y(intersection.surfaceNormal), L"normal y should be +y", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionXPlane) {
      const Plane plane{Float4{0.0f, 0.0f, 0.0f, 0.0f}, Float4{1.0f, 0.0f, 0.0f, 0.0f}};
      const Ray ray{Float4{-8.0f, 0.0f, 0.0f, 0.0f}, Float4{1.0f, 0.0f, 0.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = plane.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(8.0f, distance, L"should hit x=0 plane at distance 8", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionDiagonalRay) {
      const Plane plane{};
      const auto dir = normalize3(Float4{1.0f, 0.0f, 1.0f, 0.0f});
      const Ray ray{Float4{0.0f, 0.0f, -5.0f, 0.0f}, dir};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = plane.findNearestIntersection(rayCast, nullptr, intersection);
      const Float expected = 5.0f * std::sqrt(2.0f);

      Assert::AreEqual(expected, distance, 0.001f, L"diagonal distance to z=0", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionFromPositiveZ) {
      const Plane plane{};
      const Ray ray{Float4{0.0f, 0.0f, 5.0f, 0.0f}, Float4{0.0f, 0.0f, -1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = plane.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(5.0f, distance, L"should hit from +z side at distance 5", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionBackfaceCulled) {
      const Plane plane{};
      const Ray ray{Float4{0.0f, 0.0f, -5.0f, 0.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const auto culling = blend<false, true, false, false>(MaskAll<Float4::VectorBoolType>(), Zero<Float4::VectorBoolType>());
      const RayCast rayCast{ray, culling, Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = plane.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(100.0f, distance, L"backface culled ray hitting back of plane should miss", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionFrontfaceCulled) {
      const Plane plane{};
      const Ray ray{Float4{0.0f, 0.0f, 5.0f, 0.0f}, Float4{0.0f, 0.0f, -1.0f, 0.0f}};
      const auto culling = blend<false, true, false, false>(Zero<Float4::VectorBoolType>(), MaskAll<Float4::VectorBoolType>());
      const RayCast rayCast{ray, culling, Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = plane.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(100.0f, distance, L"frontface culled ray hitting front of plane should miss", LINE_INFO());
    }

    TEST_METHOD(findAnyIntersectionHits) {
      const Plane plane{};
      const Ray ray{Float4{0.0f, 0.0f, -5.0f, 0.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = plane.findAnyIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(5.0f, distance, L"findAny should match findNearest", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionAtExactMaxDistance) {
      const Plane plane{};
      const Ray ray{Float4{0.0f, 0.0f, -5.0f, 0.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 5.0f};
      FacetIntersection intersection{};

      const auto distance = plane.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(5.0f, distance, L"t == maxDistance is out of reach", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionJustWithinMaxDistance) {
      const Plane plane{};
      const Ray ray{Float4{0.0f, 0.0f, -5.0f, 0.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 5.001f};
      FacetIntersection intersection{};

      const auto distance = plane.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(5.0f, distance, L"t < maxDistance should hit", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionRayOriginOnPlane) {
      const Plane plane{};
      const Ray ray{Float4{0.0f, 0.0f, 0.0f, 0.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = plane.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::IsTrue(distance <= 0.0f, L"ray on plane produces t=-0 (IEEE negative zero)", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionRayOriginOnPlaneWithSelfOcclusion) {
      const Plane plane{};
      const Ray ray{Float4{0.0f, 0.0f, 0.0f, 0.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const auto originId = reinterpret_cast<ASizeT>(&plane);
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, originId), 100.0f};
      FacetIntersection intersection{};

      const auto distance = plane.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(100.0f, distance, L"self-occlusion should filter t=0 hit on originating plane", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionSelfOcclusionNearSurface) {
      const Plane plane{};
      const Ray ray{Float4{0.0f, 0.0f, -1e-8f, 0.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const auto originId = reinterpret_cast<ASizeT>(&plane);
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, originId), 100.0f};
      FacetIntersection intersection{};

      const auto distance = plane.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(100.0f, distance, L"self-occlusion should filter sub-epsilon intersection", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionSmoothedNormalMatchesSurfaceNormal) {
      const Plane plane{};
      const Ray ray{Float4{0.0f, 0.0f, -5.0f, 0.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      plane.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(z(intersection.surfaceNormal), z(intersection.smoothedNormal), L"smoothed should match surface on plane", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionModelSpaceMatchesWorldSpace) {
      const Plane plane{};
      const Ray ray{Float4{2.0f, 3.0f, -5.0f, 0.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      plane.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(x(intersection.vertex), x(intersection.msVertex), L"msVertex x should match vertex x", LINE_INFO());
      Assert::AreEqual(z(intersection.vertex), z(intersection.msVertex), L"msVertex z should match vertex z", LINE_INFO());
      Assert::AreEqual(z(intersection.surfaceNormal), z(intersection.msSurfaceNormal), L"msSurfaceNormal should match surfaceNormal", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionOnPlaneParallelProducesNaNMiss) {
      const Plane plane{};
      const Ray ray{Float4{0.0f, 0.0f, 0.0f, 0.0f}, Float4{1.0f, 0.0f, 0.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = plane.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(100.0f, distance, L"on-plane parallel ray (0/0=NaN) should miss via outOfReach", LINE_INFO());
    }
  };
}
