#include "raytracing_tests.h"

namespace raytracer::test {
  TEST_CLASS(SphereTest) {
  public:
    TEST_METHOD(constructsDefault) {
      const Sphere sphere{};
      const auto box = sphere.getBounding();
      Assert::AreEqual(-1.0f, x(box.minimum), L"default min x", LINE_INFO());
      Assert::AreEqual(1.0f, x(box.maximum), L"default max x", LINE_INFO());
    }

    TEST_METHOD(constructsFromCenterAndRadius) {
      const Sphere sphere{Float4{1.0f, 2.0f, 3.0f, 0.0f}, 5.0f};
      const auto box = sphere.getBounding();
      Assert::AreEqual(-4.0f, x(box.minimum), L"min x", LINE_INFO());
      Assert::AreEqual(6.0f, x(box.maximum), L"max x", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionHitsFromFront) {
      const Sphere sphere{Float4{0.0f, 0.0f, 0.0f, 0.0f}, 1.0f};
      const Ray ray{Float4{0.0f, 0.0f, -5.0f, 0.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = sphere.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(4.0f, distance, L"should hit front at z=-1", LINE_INFO());
      Assert::IsTrue(intersection.node == &sphere, L"node should point to sphere", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionMisses) {
      const Sphere sphere{Float4{0.0f, 0.0f, 0.0f, 0.0f}, 1.0f};
      const Ray ray{Float4{0.0f, 0.0f, -5.0f, 0.0f}, Float4{0.0f, 1.0f, 0.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = sphere.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(100.0f, distance, L"miss should return maxDistance", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionTangent) {
      const Sphere sphere{Float4{0.0f, 0.0f, 0.0f, 0.0f}, 1.0f};
      const Ray ray{Float4{0.0f, 1.0f, -5.0f, 0.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = sphere.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(5.0f, distance, 0.001f, L"tangent hit at z=0", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionFromInsideNoCulling) {
      const Sphere sphere{Float4{0.0f, 0.0f, 0.0f, 0.0f}, 1.0f};
      const Ray ray{Float4{0.0f, 0.0f, 0.0f, 0.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = sphere.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(-1.0f, distance, L"no culling returns front-face hit behind ray", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionFromInsideFrontfaceCulled) {
      const Sphere sphere{Float4{0.0f, 0.0f, 0.0f, 0.0f}, 1.0f};
      const Ray ray{Float4{0.0f, 0.0f, 0.0f, 0.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const auto culling = blend<false, true, false, false>(Zero<Float4::VectorBoolType>(), MaskAll<Float4::VectorBoolType>());
      const RayCast rayCast{ray, culling, Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = sphere.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(1.0f, distance, L"frontface culled should return backface exit at z=1", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionBeyondMaxDistance) {
      const Sphere sphere{Float4{0.0f, 0.0f, 0.0f, 0.0f}, 1.0f};
      const Ray ray{Float4{0.0f, 0.0f, -5.0f, 0.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 3.0f};
      FacetIntersection intersection{};

      const auto distance = sphere.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(3.0f, distance, L"hit beyond maxDistance should return maxDistance", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionSetsNormal) {
      const Sphere sphere{Float4{0.0f, 0.0f, 0.0f, 0.0f}, 1.0f};
      const Ray ray{Float4{0.0f, 0.0f, -5.0f, 0.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      sphere.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(0.0f, x(intersection.surfaceNormal), L"normal x", LINE_INFO());
      Assert::AreEqual(0.0f, y(intersection.surfaceNormal), L"normal y", LINE_INFO());
      Assert::AreEqual(-1.0f, z(intersection.surfaceNormal), L"normal z should point toward ray", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionSetsVertex) {
      const Sphere sphere{Float4{0.0f, 0.0f, 0.0f, 0.0f}, 1.0f};
      const Ray ray{Float4{0.0f, 0.0f, -5.0f, 0.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      sphere.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(0.0f, x(intersection.vertex), L"vertex x", LINE_INFO());
      Assert::AreEqual(0.0f, y(intersection.vertex), L"vertex y", LINE_INFO());
      Assert::AreEqual(-1.0f, z(intersection.vertex), L"vertex z", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionOffCenter) {
      const Sphere sphere{Float4{3.0f, 0.0f, 0.0f, 0.0f}, 1.0f};
      const Ray ray{Float4{3.0f, 0.0f, -5.0f, 0.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = sphere.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(4.0f, distance, L"should hit at distance 4", LINE_INFO());
      Assert::AreEqual(3.0f, x(intersection.vertex), L"vertex x should be 3", LINE_INFO());
      Assert::AreEqual(-1.0f, z(intersection.vertex), L"vertex z should be -1", LINE_INFO());
    }

    TEST_METHOD(findAnyIntersectionHits) {
      const Sphere sphere{Float4{0.0f, 0.0f, 0.0f, 0.0f}, 1.0f};
      const Ray ray{Float4{0.0f, 0.0f, -5.0f, 0.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = sphere.findAnyIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(4.0f, distance, L"findAny should return same distance as findNearest", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionRayBehindSphere) {
      const Sphere sphere{Float4{0.0f, 0.0f, 0.0f, 0.0f}, 1.0f};
      const Ray ray{Float4{0.0f, 0.0f, 5.0f, 0.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = sphere.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(100.0f, distance, L"sphere behind ray should miss", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionWithLargeRadius) {
      const Sphere sphere{Float4{0.0f, 0.0f, 0.0f, 0.0f}, 10.0f};
      const Ray ray{Float4{0.0f, 0.0f, -20.0f, 0.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = sphere.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(10.0f, distance, L"should hit at distance 10", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionDiagonal) {
      const Sphere sphere{Float4{0.0f, 0.0f, 0.0f, 0.0f}, 1.0f};
      const auto dir = normalize3(Float4{1.0f, 1.0f, 1.0f, 0.0f});
      const Ray ray{Float4{-5.0f, -5.0f, -5.0f, 0.0f}, dir};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = sphere.findNearestIntersection(rayCast, nullptr, intersection);
      const Float expected = std::sqrt(75.0f) - 1.0f;

      Assert::AreEqual(expected, distance, 0.001f, L"diagonal distance", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionBackfaceCulledFromInside) {
      const Sphere sphere{Float4{0.0f, 0.0f, 0.0f, 0.0f}, 1.0f};
      const Ray ray{Float4{0.0f, 0.0f, 0.0f, 0.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const auto culling = blend<false, true, false, false>(MaskAll<Float4::VectorBoolType>(), Zero<Float4::VectorBoolType>());
      const RayCast rayCast{ray, culling, Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = sphere.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(-1.0f, distance, L"backface culled blocks exit but frontface at t=-1 still returned", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionBackfaceCulledFromOutside) {
      const Sphere sphere{Float4{0.0f, 0.0f, 0.0f, 0.0f}, 1.0f};
      const Ray ray{Float4{0.0f, 0.0f, -5.0f, 0.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const auto culling = blend<false, true, false, false>(MaskAll<Float4::VectorBoolType>(), Zero<Float4::VectorBoolType>());
      const RayCast rayCast{ray, culling, Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = sphere.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(4.0f, distance, L"backface culled still returns frontface hit", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionAtExactMaxDistance) {
      const Sphere sphere{Float4{0.0f, 0.0f, 0.0f, 0.0f}, 1.0f};
      const Ray ray{Float4{0.0f, 0.0f, -5.0f, 0.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 4.0f};
      FacetIntersection intersection{};

      const auto distance = sphere.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(4.0f, distance, L"t == maxDistance is out of reach", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionJustWithinMaxDistance) {
      const Sphere sphere{Float4{0.0f, 0.0f, 0.0f, 0.0f}, 1.0f};
      const Ray ray{Float4{0.0f, 0.0f, -5.0f, 0.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 4.001f};
      FacetIntersection intersection{};

      const auto distance = sphere.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(4.0f, distance, L"t < maxDistance should hit", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionSetsTexCoords) {
      const Sphere sphere{Float4{0.0f, 0.0f, 0.0f, 0.0f}, 1.0f};
      const Ray ray{Float4{0.0f, 0.0f, -5.0f, 0.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      sphere.findNearestIntersection(rayCast, nullptr, intersection);

      // vectorization::atan2(z,x) wraps std::atan2(x,z): atan2(0,-1)=π → u = π*(1/2π)+0.5 = 1.0
      Assert::AreEqual(1.0f, x(intersection.texCoords), 0.001f, L"texCoord u for -z hit", LINE_INFO());
      Assert::AreEqual(0.5f, y(intersection.texCoords), 0.001f, L"texCoord v for equator hit", LINE_INFO());
      Assert::AreEqual(1.0f, w(intersection.texCoords), L"texCoord w should be 1", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionTexCoordsForXHit) {
      const Sphere sphere{Float4{0.0f, 0.0f, 0.0f, 0.0f}, 1.0f};
      const Ray ray{Float4{-5.0f, 0.0f, 0.0f, 0.0f}, Float4{1.0f, 0.0f, 0.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      sphere.findNearestIntersection(rayCast, nullptr, intersection);

      // vectorization::atan2(0,-1) wraps std::atan2(-1,0)=-π/2 → u = -π/2*(1/2π)+0.5 = 0.25
      Assert::AreEqual(0.25f, x(intersection.texCoords), 0.001f, L"texCoord u for -x hit", LINE_INFO());
      Assert::AreEqual(0.5f, y(intersection.texCoords), 0.001f, L"texCoord v for equator hit", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionOnSurfaceNoSelfOcclusion) {
      const Sphere sphere{Float4{0.0f, 0.0f, 0.0f, 0.0f}, 1.0f};
      const Ray ray{Float4{0.0f, 0.0f, -1.0f, 0.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = sphere.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(0.0f, distance, L"ray starting on surface should hit at t=0", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionOnSurfaceWithSelfOcclusion) {
      const Sphere sphere{Float4{0.0f, 0.0f, 0.0f, 0.0f}, 1.0f};
      const Ray ray{Float4{0.0f, 0.0f, -1.0f, 0.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const auto originId = reinterpret_cast<ASizeT>(&sphere);
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, originId), 100.0f};
      FacetIntersection intersection{};

      const auto distance = sphere.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(2.0f, distance, L"self-occlusion should skip t=0 hit, return t=2", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionSmoothedNormalMatchesSurfaceNormal) {
      const Sphere sphere{Float4{0.0f, 0.0f, 0.0f, 0.0f}, 1.0f};
      const Ray ray{Float4{0.0f, 0.0f, -5.0f, 0.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      sphere.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(z(intersection.surfaceNormal), z(intersection.smoothedNormal), L"smoothed should match surface on sphere", LINE_INFO());
      Assert::AreEqual(x(intersection.surfaceNormal), x(intersection.smoothedNormal), L"smoothed x should match surface", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionModelSpaceMatchesWorldSpace) {
      const Sphere sphere{Float4{0.0f, 0.0f, 0.0f, 0.0f}, 1.0f};
      const Ray ray{Float4{0.0f, 0.0f, -5.0f, 0.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      sphere.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(z(intersection.vertex), z(intersection.msVertex), L"msVertex z should match vertex z", LINE_INFO());
      Assert::AreEqual(z(intersection.surfaceNormal), z(intersection.msSurfaceNormal), L"msSurfaceNormal should match surfaceNormal", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionNaNDirectionMisses) {
      const Sphere sphere{Float4{0.0f, 0.0f, 0.0f, 0.0f}, 1.0f};
      const auto nan = std::numeric_limits<Float>::quiet_NaN();
      const Ray ray{Float4{0.0f, 0.0f, -5.0f, 0.0f}, Float4{nan, 0.0f, 0.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = sphere.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(100.0f, distance, L"NaN direction should miss", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionInfinityOriginMisses) {
      const Sphere sphere{Float4{0.0f, 0.0f, 0.0f, 0.0f}, 1.0f};
      const auto inf = std::numeric_limits<Float>::infinity();
      const Ray ray{Float4{inf, inf, inf, 0.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = sphere.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(100.0f, distance, L"infinity origin should miss", LINE_INFO());
    }
  };
}
