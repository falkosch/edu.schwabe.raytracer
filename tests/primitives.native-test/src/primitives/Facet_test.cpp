#include "primitives_tests.h"

namespace primitives::test {
  TEST_CLASS(FacetTest) {
  public:
    TEST_METHOD(constructsDefault) {
      const Facet facet{};
      Assert::IsTrue(allTrue(facet.v0 == Zero<Float4>()), L"v0 should be zero", LINE_INFO());
      Assert::IsTrue(allTrue(facet.v1 == Zero<Float4>()), L"v1 should be zero", LINE_INFO());
      Assert::IsTrue(allTrue(facet.v2 == Zero<Float4>()), L"v2 should be zero", LINE_INFO());
    }

    TEST_METHOD(constructsFromSingleVertex) {
      const Float4 v{1.0f, 2.0f, 3.0f, 0.0f};
      const Facet facet{v};
      Assert::IsTrue(allTrue(facet.v0 == v), L"v0 should match", LINE_INFO());
      Assert::IsTrue(allTrue(facet.v1 == v), L"v1 should match", LINE_INFO());
      Assert::IsTrue(allTrue(facet.v2 == v), L"v2 should match", LINE_INFO());
    }

    TEST_METHOD(constructsFromThreeVertices) {
      const Float4 a{1.0f, 0.0f, 0.0f, 0.0f};
      const Float4 b{0.0f, 1.0f, 0.0f, 0.0f};
      const Float4 c{0.0f, 0.0f, 1.0f, 0.0f};
      const Facet facet{a, b, c};
      Assert::IsTrue(allTrue(facet.v0 == a), L"v0 should match", LINE_INFO());
      Assert::IsTrue(allTrue(facet.v1 == b), L"v1 should match", LINE_INFO());
      Assert::IsTrue(allTrue(facet.v2 == c), L"v2 should match", LINE_INFO());
    }

    TEST_METHOD(computesBounding) {
      const Float4 v0{0.0f, 0.0f, 0.0f, 0.0f};
      const Float4 v1{1.0f, 0.0f, 0.0f, 0.0f};
      const Float4 v2{0.0f, 1.0f, 0.0f, 0.0f};
      const Facet facet{v0, v1, v2};
      const auto box = bounding(facet);
      Assert::AreEqual(0.0f, x(box.minimum), L"min x should be 0", LINE_INFO());
      Assert::AreEqual(0.0f, y(box.minimum), L"min y should be 0", LINE_INFO());
      Assert::AreEqual(0.0f, z(box.minimum), L"min z should be 0", LINE_INFO());
      Assert::AreEqual(1.0f, x(box.maximum), L"max x should be 1", LINE_INFO());
      Assert::AreEqual(1.0f, y(box.maximum), L"max y should be 1", LINE_INFO());
      Assert::AreEqual(0.0f, z(box.maximum), L"max z should be 0", LINE_INFO());
    }

    TEST_METHOD(translatesForward) {
      const Float4 v0{0.0f, 0.0f, 0.0f, 0.0f};
      const Float4 v1{1.0f, 0.0f, 0.0f, 0.0f};
      const Float4 v2{0.0f, 1.0f, 0.0f, 0.0f};
      const Facet facet{v0, v1, v2};
      const Float4 offset{1.0f, 2.0f, 3.0f, 0.0f};
      const auto result = facet + offset;
      Assert::IsTrue(allTrue(result.v0 == (v0 + offset)), L"v0 should be translated", LINE_INFO());
      Assert::IsTrue(allTrue(result.v1 == (v1 + offset)), L"v1 should be translated", LINE_INFO());
      Assert::IsTrue(allTrue(result.v2 == (v2 + offset)), L"v2 should be translated", LINE_INFO());
    }

    TEST_METHOD(translatesBackward) {
      const Float4 v0{1.0f, 2.0f, 3.0f, 0.0f};
      const Float4 v1{2.0f, 2.0f, 3.0f, 0.0f};
      const Float4 v2{1.0f, 3.0f, 3.0f, 0.0f};
      const Facet facet{v0, v1, v2};
      const Float4 offset{1.0f, 2.0f, 3.0f, 0.0f};
      const auto result = facet - offset;
      Assert::IsTrue(allTrue(result.v0 == (v0 - offset)), L"v0 should be translated back", LINE_INFO());
      Assert::IsTrue(allTrue(result.v1 == (v1 - offset)), L"v1 should be translated back", LINE_INFO());
      Assert::IsTrue(allTrue(result.v2 == (v2 - offset)), L"v2 should be translated back", LINE_INFO());
    }

    TEST_METHOD(computesBaryCenter) {
      const Float4 v0{0.0f, 0.0f, 0.0f, 0.0f};
      const Float4 v1{1.0f, 0.0f, 0.0f, 0.0f};
      const Float4 v2{0.0f, 1.0f, 0.0f, 0.0f};
      const Facet facet{v0, v1, v2};

      // uv=(0,0) -> v0
      const Float4 uv0{0.0f, 0.0f, 0.0f, 0.0f};
      Assert::IsTrue(allTrue(baryCenter(uv0, facet) == v0), L"uv(0,0) should give v0", LINE_INFO());

      // uv=(1,0) -> v1
      const Float4 uv1{1.0f, 0.0f, 0.0f, 0.0f};
      Assert::IsTrue(allTrue(baryCenter(uv1, facet) == v1), L"uv(1,0) should give v1", LINE_INFO());

      // uv=(0,1) -> v2
      const Float4 uv2{0.0f, 1.0f, 0.0f, 0.0f};
      Assert::IsTrue(allTrue(baryCenter(uv2, facet) == v2), L"uv(0,1) should give v2", LINE_INFO());
    }

    TEST_METHOD(intersectsRayHavelHit) {
      const Float4 v0{0.0f, 0.0f, 0.0f, 0.0f};
      const Float4 v1{1.0f, 0.0f, 0.0f, 0.0f};
      const Float4 v2{0.0f, 1.0f, 0.0f, 0.0f};

      const auto edge1 = v1 - v0;
      const auto edge2 = v2 - v0;
      auto planeNormal0 = cross3(edge1, edge2);
      auto planeNormal1 = cross3(edge2, planeNormal0);
      auto planeNormal2 = cross3(planeNormal0, edge1);
      FacetNormals planeNormals{
          replaceW(planeNormal0, -dot3(v0, planeNormal0)),
          replaceW(planeNormal1, -dot3(v0, planeNormal1)),
          replaceW(planeNormal2, -dot3(v0, planeNormal2))
      };
      planeNormals.v1 /= dotv(planeNormals.v1, v1);
      planeNormals.v2 /= dotv(planeNormals.v2, v2);

      const Float4 origin{0.25f, 0.25f, -1.0f, 0.0f};
      const Float4 direction{0.0f, 0.0f, 1.0f, 0.0f};
      const Ray ray{origin, direction};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      const Float4 maxDistance{100.0f, 100.0f, 100.0f, 100.0f};

      const auto result = nearestIntersectionHavel(planeNormals, rayCast, maxDistance);
      Assert::AreEqual(1.0f, w(result), 0.001f, L"distance should be ~1.0", LINE_INFO());
      Assert::AreEqual(0.25f, x(result), 0.001f, L"u should be ~0.25", LINE_INFO());
      Assert::AreEqual(0.25f, y(result), 0.001f, L"v should be ~0.25", LINE_INFO());
    }

    TEST_METHOD(intersectsRayHavelMiss) {
      const Float4 v0{0.0f, 0.0f, 0.0f, 0.0f};
      const Float4 v1{1.0f, 0.0f, 0.0f, 0.0f};
      const Float4 v2{0.0f, 1.0f, 0.0f, 0.0f};

      const auto edge1 = v1 - v0;
      const auto edge2 = v2 - v0;
      auto planeNormal0 = cross3(edge1, edge2);
      auto planeNormal1 = cross3(edge2, planeNormal0);
      auto planeNormal2 = cross3(planeNormal0, edge1);
      FacetNormals planeNormals{
          replaceW(planeNormal0, -dot3(v0, planeNormal0)),
          replaceW(planeNormal1, -dot3(v0, planeNormal1)),
          replaceW(planeNormal2, -dot3(v0, planeNormal2))
      };
      planeNormals.v1 /= dotv(planeNormals.v1, v1);
      planeNormals.v2 /= dotv(planeNormals.v2, v2);

      const Float4 origin{5.0f, 5.0f, -1.0f, 0.0f};
      const Float4 direction{0.0f, 0.0f, 1.0f, 0.0f};
      const Ray ray{origin, direction};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      const Float4 maxDistance{100.0f, 100.0f, 100.0f, 100.0f};

      const auto result = nearestIntersectionHavel(planeNormals, rayCast, maxDistance);
      Assert::IsTrue(allTrue(result == maxDistance), L"miss should return maxDistance", LINE_INFO());
    }
  };
}
