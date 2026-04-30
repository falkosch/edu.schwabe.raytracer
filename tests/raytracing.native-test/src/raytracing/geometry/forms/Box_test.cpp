#include "raytracing_tests.h"

namespace raytracer::test {
  TEST_CLASS(BoxTest) {
  public:
    TEST_METHOD(constructsDefault) {
      const Box box{};
      const auto bounding = box.getBounding();
      Assert::AreEqual(-1.0f, x(bounding.minimum), L"default min x", LINE_INFO());
      Assert::AreEqual(-1.0f, y(bounding.minimum), L"default min y", LINE_INFO());
      Assert::AreEqual(-1.0f, z(bounding.minimum), L"default min z", LINE_INFO());
      Assert::AreEqual(1.0f, x(bounding.maximum), L"default max x", LINE_INFO());
      Assert::AreEqual(1.0f, y(bounding.maximum), L"default max y", LINE_INFO());
      Assert::AreEqual(1.0f, z(bounding.maximum), L"default max z", LINE_INFO());
    }

    TEST_METHOD(constructsFromMinMax) {
      const Box box{Float4{-2.0f, -3.0f, -4.0f, 0.0f}, Float4{2.0f, 3.0f, 4.0f, 0.0f}};
      const auto bounding = box.getBounding();
      Assert::AreEqual(-2.0f, x(bounding.minimum), L"min x", LINE_INFO());
      Assert::AreEqual(4.0f, z(bounding.maximum), L"max z", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionHitsZFace) {
      const Box box{};
      const Ray ray{Float4{0.0f, 0.0f, -5.0f, 0.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = box.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(4.0f, distance, L"should hit -z face at distance 4", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionHitsXFace) {
      const Box box{};
      const Ray ray{Float4{5.0f, 0.0f, 0.0f, 0.0f}, Float4{-1.0f, 0.0f, 0.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = box.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(4.0f, distance, L"should hit +x face at distance 4", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionHitsYFace) {
      const Box box{};
      const Ray ray{Float4{0.0f, -5.0f, 0.0f, 0.0f}, Float4{0.0f, 1.0f, 0.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = box.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(4.0f, distance, L"should hit -y face at distance 4", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionMisses) {
      const Box box{};
      const Ray ray{Float4{0.0f, 0.0f, -5.0f, 0.0f}, Float4{0.0f, 1.0f, 0.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = box.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(100.0f, distance, L"miss should return maxDistance", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionMissesDiagonally) {
      const Box box{};
      const auto dir = normalize3(Float4{0.0f, 1.01f, 1.0f, 0.0f});
      const Ray ray{Float4{0.0f, 0.0f, -5.0f, 0.0f}, dir};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = box.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(100.0f, distance, L"ray angled above box should miss", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionBeyondMaxDistance) {
      const Box box{};
      const Ray ray{Float4{0.0f, 0.0f, -5.0f, 0.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 3.0f};
      FacetIntersection intersection{};

      const auto distance = box.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(3.0f, distance, L"hit beyond maxDistance should return maxDistance", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionRayBehindBox) {
      const Box box{};
      const Ray ray{Float4{0.0f, 0.0f, 5.0f, 0.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = box.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(100.0f, distance, L"box behind ray should miss", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionFromInsideFrontfaceCulled) {
      const Box box{};
      const Ray ray{Float4{0.0f, 0.0f, 0.0f, 0.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const auto culling = blend<false, true, false, false>(Zero<Float4::VectorBoolType>(), MaskAll<Float4::VectorBoolType>());
      const RayCast rayCast{ray, culling, Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = box.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(1.0f, distance, L"frontface culled should return backface exit at z=1", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionSetsNormalZFace) {
      const Box box{};
      const Ray ray{Float4{0.0f, 0.0f, -5.0f, 0.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      box.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(0.0f, x(intersection.surfaceNormal), L"normal x", LINE_INFO());
      Assert::AreEqual(0.0f, y(intersection.surfaceNormal), L"normal y", LINE_INFO());
      Assert::AreEqual(-1.0f, z(intersection.surfaceNormal), L"normal z should face -z", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionSetsNormalXFace) {
      const Box box{};
      const Ray ray{Float4{5.0f, 0.0f, 0.0f, 0.0f}, Float4{-1.0f, 0.0f, 0.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      box.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(1.0f, x(intersection.surfaceNormal), L"normal x should face +x", LINE_INFO());
      Assert::AreEqual(0.0f, y(intersection.surfaceNormal), L"normal y", LINE_INFO());
      Assert::AreEqual(0.0f, z(intersection.surfaceNormal), L"normal z", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionSetsVertex) {
      const Box box{};
      const Ray ray{Float4{0.0f, 0.0f, -5.0f, 0.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      box.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(0.0f, x(intersection.vertex), L"vertex x", LINE_INFO());
      Assert::AreEqual(0.0f, y(intersection.vertex), L"vertex y", LINE_INFO());
      Assert::AreEqual(-1.0f, z(intersection.vertex), L"vertex z", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionSetsNode) {
      const Box box{};
      const Ray ray{Float4{0.0f, 0.0f, -5.0f, 0.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      box.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::IsTrue(intersection.node == &box, L"node should point to box", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionOffCenter) {
      const Box box{Float4{2.0f, 2.0f, 2.0f, 0.0f}, Float4{4.0f, 4.0f, 4.0f, 0.0f}};
      const Ray ray{Float4{3.0f, 3.0f, -5.0f, 0.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = box.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(7.0f, distance, L"should hit -z face at distance 7", LINE_INFO());
    }

    TEST_METHOD(findAnyIntersectionHits) {
      const Box box{};
      const Ray ray{Float4{0.0f, 0.0f, -5.0f, 0.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = box.findAnyIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(4.0f, distance, L"findAny should match findNearest", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionDiagonal) {
      const Box box{};
      const auto dir = normalize3(Float4{1.0f, 1.0f, 1.0f, 0.0f});
      const Ray ray{Float4{-5.0f, -5.0f, -5.0f, 0.0f}, dir};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = box.findNearestIntersection(rayCast, nullptr, intersection);
      const Float expected = 4.0f * std::sqrt(3.0f);

      Assert::AreEqual(expected, distance, 0.001f, L"diagonal distance to corner", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionEdgeHit) {
      const Box box{};
      const Ray ray{Float4{1.0f, 0.0f, -5.0f, 0.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = box.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(4.0f, distance, L"hitting box edge along z should still intersect", LINE_INFO());
      Assert::AreEqual(-1.0f, z(intersection.vertex), L"vertex z should be on -z face", LINE_INFO());
      Assert::AreEqual(1.0f, x(intersection.vertex), L"vertex x should be on edge", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionSetsNormalPositiveZFace) {
      const Box box{};
      const Ray ray{Float4{0.0f, 0.0f, 5.0f, 0.0f}, Float4{0.0f, 0.0f, -1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      box.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(0.0f, x(intersection.surfaceNormal), L"normal x", LINE_INFO());
      Assert::AreEqual(0.0f, y(intersection.surfaceNormal), L"normal y", LINE_INFO());
      Assert::AreEqual(1.0f, z(intersection.surfaceNormal), L"normal z should face +z", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionSetsNormalNegativeXFace) {
      const Box box{};
      const Ray ray{Float4{-5.0f, 0.0f, 0.0f, 0.0f}, Float4{1.0f, 0.0f, 0.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      box.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(-1.0f, x(intersection.surfaceNormal), L"normal x should face -x", LINE_INFO());
      Assert::AreEqual(0.0f, y(intersection.surfaceNormal), L"normal y", LINE_INFO());
      Assert::AreEqual(0.0f, z(intersection.surfaceNormal), L"normal z", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionSetsNormalPositiveYFace) {
      const Box box{};
      const Ray ray{Float4{0.0f, 5.0f, 0.0f, 0.0f}, Float4{0.0f, -1.0f, 0.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      box.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(0.0f, x(intersection.surfaceNormal), L"normal x", LINE_INFO());
      Assert::AreEqual(1.0f, y(intersection.surfaceNormal), L"normal y should face +y", LINE_INFO());
      Assert::AreEqual(0.0f, z(intersection.surfaceNormal), L"normal z", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionSetsNormalNegativeYFace) {
      const Box box{};
      const Ray ray{Float4{0.0f, -5.0f, 0.0f, 0.0f}, Float4{0.0f, 1.0f, 0.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      box.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(0.0f, x(intersection.surfaceNormal), L"normal x", LINE_INFO());
      Assert::AreEqual(-1.0f, y(intersection.surfaceNormal), L"normal y should face -y", LINE_INFO());
      Assert::AreEqual(0.0f, z(intersection.surfaceNormal), L"normal z", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionBackfaceCulled) {
      const Box box{};
      const Ray ray{Float4{0.0f, 0.0f, 0.0f, 0.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const auto culling = blend<false, true, false, false>(MaskAll<Float4::VectorBoolType>(), Zero<Float4::VectorBoolType>());
      const RayCast rayCast{ray, culling, Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = box.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(100.0f, distance, L"backface culled from inside should miss both hits", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionAtExactMaxDistance) {
      const Box box{};
      const Ray ray{Float4{0.0f, 0.0f, -5.0f, 0.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 4.0f};
      FacetIntersection intersection{};

      const auto distance = box.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(4.0f, distance, L"t == maxDistance is out of reach", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionSetsTexCoords) {
      const Box box{};
      const Ray ray{Float4{0.0f, 0.0f, -5.0f, 0.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      box.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::IsTrue(x(intersection.texCoords) >= 0.0f && x(intersection.texCoords) <= 1.0f, L"texCoord u in [0,1]", LINE_INFO());
      Assert::IsTrue(y(intersection.texCoords) >= 0.0f && y(intersection.texCoords) <= 1.0f, L"texCoord v in [0,1]", LINE_INFO());
      Assert::AreEqual(1.0f, w(intersection.texCoords), L"texCoord w should be 1", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionTexCoordsVaryWithHitPosition) {
      const Box box{};
      const RayCast rayCastZ{
          Ray{Float4{0.0f, 0.0f, -5.0f, 0.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}},
          Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f
      };
      const RayCast rayCastX{
          Ray{Float4{5.0f, 0.0f, 0.0f, 0.0f}, Float4{-1.0f, 0.0f, 0.0f, 0.0f}},
          Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f
      };
      FacetIntersection intersectionZ{}, intersectionX{};

      box.findNearestIntersection(rayCastZ, nullptr, intersectionZ);
      box.findNearestIntersection(rayCastX, nullptr, intersectionX);

      Assert::AreNotEqual(x(intersectionZ.texCoords), x(intersectionX.texCoords), L"u should differ for different faces", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionOnSurfaceWithSelfOcclusion) {
      const Box box{};
      const Ray ray{Float4{0.0f, 0.0f, -1.0f, 0.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const auto originId = reinterpret_cast<ASizeT>(&box);
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, originId), 100.0f};
      FacetIntersection intersection{};

      const auto distance = box.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(2.0f, distance, L"self-occlusion skips near face, returns far face at z=1", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionSmoothedNormalMatchesSurfaceNormal) {
      const Box box{};
      const Ray ray{Float4{0.0f, 0.0f, -5.0f, 0.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      box.findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(z(intersection.surfaceNormal), z(intersection.smoothedNormal), L"smoothed should match surface on box", LINE_INFO());
    }
  };
}
