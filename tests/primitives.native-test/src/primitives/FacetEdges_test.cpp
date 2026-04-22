#include "primitives_tests.h"

namespace primitives::test {
  TEST_CLASS(FacetEdgesTest) {
  public:
    TEST_METHOD(constructsDefault) {
      const FacetEdges edges{};
      Assert::IsTrue(allTrue(edges.edge1 == Zero<Float4>()), L"edge1 should be zero", LINE_INFO());
      Assert::IsTrue(allTrue(edges.edge2 == Zero<Float4>()), L"edge2 should be zero", LINE_INFO());
    }

    TEST_METHOD(constructsFromEdges) {
      const Float4 e1{1.0f, 0.0f, 0.0f, 0.0f};
      const Float4 e2{0.0f, 1.0f, 0.0f, 0.0f};
      const FacetEdges edges{e1, e2};
      Assert::IsTrue(allTrue(edges.edge1 == e1), L"edge1 should match", LINE_INFO());
      Assert::IsTrue(allTrue(edges.edge2 == e2), L"edge2 should match", LINE_INFO());
    }

    TEST_METHOD(intersectsRayMoellerHit) {
      const Float4 v0{0.0f, 0.0f, 0.0f, 0.0f};
      const Float4 e1{1.0f, 0.0f, 0.0f, 0.0f};
      const Float4 e2{0.0f, 1.0f, 0.0f, 0.0f};
      const FacetEdges edges{e1, e2};

      const Float4 origin{0.25f, 0.25f, -1.0f, 0.0f};
      const Float4 direction{0.0f, 0.0f, 1.0f, 0.0f};
      const Ray ray{origin, direction};
      const Float4 maxDistance{100.0f, 100.0f, 100.0f, 100.0f};

      const auto result = nearestIntersectionMoeller(v0, edges, ray, maxDistance);
      Assert::AreEqual(0.25f, x(result), 0.001f, L"u should be ~0.25", LINE_INFO());
      Assert::AreEqual(0.25f, y(result), 0.001f, L"v should be ~0.25", LINE_INFO());
      Assert::AreEqual(1.0f, z(result), 0.001f, L"distance should be ~1.0", LINE_INFO());
    }

    TEST_METHOD(intersectsRayMoellerMiss) {
      const Float4 v0{0.0f, 0.0f, 0.0f, 0.0f};
      const Float4 e1{1.0f, 0.0f, 0.0f, 0.0f};
      const Float4 e2{0.0f, 1.0f, 0.0f, 0.0f};
      const FacetEdges edges{e1, e2};

      const Float4 origin{5.0f, 5.0f, -1.0f, 0.0f};
      const Float4 direction{0.0f, 0.0f, 1.0f, 0.0f};
      const Ray ray{origin, direction};
      const Float4 maxDistance{100.0f, 100.0f, 100.0f, 100.0f};

      const auto result = nearestIntersectionMoeller(v0, edges, ray, maxDistance);
      Assert::IsTrue(allTrue(result == maxDistance), L"miss should return maxDistance", LINE_INFO());
    }
  };
}
