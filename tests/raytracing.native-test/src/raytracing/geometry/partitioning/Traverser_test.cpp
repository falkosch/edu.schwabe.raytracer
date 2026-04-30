#include "raytracing_tests.h"

#include "raytracing/geometry/partitioning/treebalancer/MedianKDTreeBalancer.h"
#include "raytracing/geometry/partitioning/treetraverser/NaiveKDTreeTraverser.h"

namespace raytracer::test {
  // A minimal GeometryNodesTraverser over Box leaves: at each leaf, iterate the boxes and pick the
  // nearest hit. This lets us drive a NaiveKDTreeTraverser end-to-end without going through Mesh.
  class BoxNodesTraverser : public GeometryNodesTraverser<FacetIntersection> {
  public:
    Float findNearestIntersection(
        const PGeometryNodeList &geometryNodes, const RayCast &rayCast, const FacetIntersection *const origin,
        FacetIntersection &intersectionOut
    ) const override {
      Float nearest = rayCast.maxDistance;
      for (auto *node : geometryNodes) {
        const auto *box = static_cast<const Box *>(node);
        if (origin && origin->node == box) {
          continue;
        }
        FacetIntersection candidate{};
        RayCast scoped{rayCast};
        scoped.maxDistance = nearest;
        const Float d = box->findNearestIntersection(scoped, origin, candidate);
        if (d < nearest) {
          nearest = d;
          intersectionOut = candidate;
          intersectionOut.node = box;
        }
      }
      return nearest;
    }

    Float findAnyIntersection(
        const PGeometryNodeList &geometryNodes, const RayCast &rayCast, const FacetIntersection *const origin,
        FacetIntersection &intersectionOut
    ) const override {
      for (auto *node : geometryNodes) {
        const auto *box = static_cast<const Box *>(node);
        if (origin && origin->node == box) {
          continue;
        }
        FacetIntersection candidate{};
        const Float d = box->findAnyIntersection(rayCast, origin, candidate);
        if (d < rayCast.maxDistance) {
          intersectionOut = candidate;
          intersectionOut.node = box;
          return d;
        }
      }
      return rayCast.maxDistance;
    }
  };

  TEST_CLASS(TraverserTest) {
    struct BoxTreeFixture {
      std::vector<std::unique_ptr<Box>> ownedBoxes;
      PGeometryNodeList nodes;
      std::unique_ptr<KDTreeRoot> root;
      BoxNodesTraverser nodesTraverser;
    };

    static BoxTreeFixture makeThreeBoxesOnXAxis() {
      BoxTreeFixture fix;
      // three boxes spaced along x at -3, 0, 3
      const Float4 size{0.5f, 0.5f, 0.5f, 0.0f};
      for (Float cx : {-3.0f, 0.0f, 3.0f}) {
        const auto center = Float4{cx, 0.0f, 0.0f, 0.0f};
        auto box = std::make_unique<Box>(center - size, center + size);
        fix.nodes.push_back(box.get());
        fix.ownedBoxes.push_back(std::move(box));
      }
      const MedianKDTreeBalancer balancer{};
      fix.root = balancer.build(fix.nodes);
      return fix;
    }

  public:
    TEST_METHOD(naiveTraverserNearestHitsClosestBox) {
      auto fix = makeThreeBoxesOnXAxis();
      const NaiveKDTreeTraverser<FacetIntersection> traverser{};
      // ray along +x from x=-10; first hit is the leftmost box at x=-3.5, distance 6.5
      const Ray ray{Float4{-10.0f, 0.0f, 0.0f, 0.0f}, Float4{1.0f, 0.0f, 0.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = traverser.findNearestIntersection(fix.nodesTraverser, *fix.root, rayCast, nullptr, intersection);

      Assert::AreEqual(6.5f, distance, 0.001f, L"closest box hit at x=-3.5", LINE_INFO());
    }

    TEST_METHOD(naiveTraverserNearestPicksNearestNotFirst) {
      auto fix = makeThreeBoxesOnXAxis();
      const NaiveKDTreeTraverser<FacetIntersection> traverser{};
      // ray from +x going toward -x; nearest hit must be rightmost box first, not leftmost
      const Ray ray{Float4{10.0f, 0.0f, 0.0f, 0.0f}, Float4{-1.0f, 0.0f, 0.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = traverser.findNearestIntersection(fix.nodesTraverser, *fix.root, rayCast, nullptr, intersection);

      Assert::AreEqual(6.5f, distance, 0.001f, L"rightmost box hit first at x=3.5", LINE_INFO());
    }

    TEST_METHOD(naiveTraverserNearestMissReturnsMaxDistance) {
      auto fix = makeThreeBoxesOnXAxis();
      const NaiveKDTreeTraverser<FacetIntersection> traverser{};
      // ray that passes above every box
      const Ray ray{Float4{-10.0f, 5.0f, 0.0f, 0.0f}, Float4{1.0f, 0.0f, 0.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = traverser.findNearestIntersection(fix.nodesTraverser, *fix.root, rayCast, nullptr, intersection);

      Assert::AreEqual(100.0f, distance, L"miss returns maxDistance", LINE_INFO());
    }

    TEST_METHOD(naiveTraverserMissesEntireTreeBoundsEarly) {
      auto fix = makeThreeBoxesOnXAxis();
      const NaiveKDTreeTraverser<FacetIntersection> traverser{};
      // ray that never enters root bounding (root spans x ∈ [-3.5, 3.5], y ∈ [-0.5, 0.5])
      const Ray ray{Float4{0.0f, 100.0f, 0.0f, 0.0f}, Float4{0.0f, 1.0f, 0.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 50.0f};
      FacetIntersection intersection{};

      const auto distance = traverser.findNearestIntersection(fix.nodesTraverser, *fix.root, rayCast, nullptr, intersection);

      Assert::AreEqual(50.0f, distance, L"ray pointing away from tree bounds returns maxDistance", LINE_INFO());
    }

    TEST_METHOD(naiveTraverserRayStartingInsideTreeBoundsHits) {
      auto fix = makeThreeBoxesOnXAxis();
      const NaiveKDTreeTraverser<FacetIntersection> traverser{};
      // origin between the leftmost and middle box; nearest hit goes forward into middle box
      const Ray ray{Float4{-1.5f, 0.0f, 0.0f, 0.0f}, Float4{1.0f, 0.0f, 0.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = traverser.findNearestIntersection(fix.nodesTraverser, *fix.root, rayCast, nullptr, intersection);

      // middle box at x ∈ [-0.5, 0.5]; from x=-1.5 the entry is at x=-0.5, distance 1.0
      Assert::AreEqual(1.0f, distance, 0.001f, L"interior origin hits next box at distance 1", LINE_INFO());
    }

    TEST_METHOD(naiveTraverserMaxDistancePrunes) {
      auto fix = makeThreeBoxesOnXAxis();
      const NaiveKDTreeTraverser<FacetIntersection> traverser{};
      // ray that would hit at distance 6.5 but maxDistance=2 forbids it
      const Ray ray{Float4{-10.0f, 0.0f, 0.0f, 0.0f}, Float4{1.0f, 0.0f, 0.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 2.0f};
      FacetIntersection intersection{};

      const auto distance = traverser.findNearestIntersection(fix.nodesTraverser, *fix.root, rayCast, nullptr, intersection);

      Assert::AreEqual(2.0f, distance, L"hit at 6.5 beyond maxDistance=2 returns maxDistance", LINE_INFO());
    }

    TEST_METHOD(naiveTraverserMaxDistanceJustWithinReturnsHit) {
      auto fix = makeThreeBoxesOnXAxis();
      const NaiveKDTreeTraverser<FacetIntersection> traverser{};
      const Ray ray{Float4{-10.0f, 0.0f, 0.0f, 0.0f}, Float4{1.0f, 0.0f, 0.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 6.501f};
      FacetIntersection intersection{};

      const auto distance = traverser.findNearestIntersection(fix.nodesTraverser, *fix.root, rayCast, nullptr, intersection);

      Assert::AreEqual(6.5f, distance, 0.001f, L"hit just within maxDistance", LINE_INFO());
    }

    TEST_METHOD(naiveTraverserAnyHitFinds) {
      auto fix = makeThreeBoxesOnXAxis();
      const NaiveKDTreeTraverser<FacetIntersection> traverser{};
      const Ray ray{Float4{-10.0f, 0.0f, 0.0f, 0.0f}, Float4{1.0f, 0.0f, 0.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = traverser.findAnyIntersection(fix.nodesTraverser, *fix.root, rayCast, nullptr, intersection);

      Assert::IsTrue(distance < 100.0f, L"findAny should find a hit", LINE_INFO());
      Assert::IsTrue(distance >= 6.5f - 0.001f, L"hit no closer than nearest", LINE_INFO());
    }

    TEST_METHOD(naiveTraverserAnyHitMisses) {
      auto fix = makeThreeBoxesOnXAxis();
      const NaiveKDTreeTraverser<FacetIntersection> traverser{};
      const Ray ray{Float4{-10.0f, 5.0f, 0.0f, 0.0f}, Float4{1.0f, 0.0f, 0.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = traverser.findAnyIntersection(fix.nodesTraverser, *fix.root, rayCast, nullptr, intersection);

      Assert::AreEqual(100.0f, distance, L"findAny miss returns maxDistance", LINE_INFO());
    }

    TEST_METHOD(naiveTraverserAnyHitMaxDistanceExcludesHit) {
      auto fix = makeThreeBoxesOnXAxis();
      const NaiveKDTreeTraverser<FacetIntersection> traverser{};
      const Ray ray{Float4{-10.0f, 0.0f, 0.0f, 0.0f}, Float4{1.0f, 0.0f, 0.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 5.0f};
      FacetIntersection intersection{};

      const auto distance = traverser.findAnyIntersection(fix.nodesTraverser, *fix.root, rayCast, nullptr, intersection);

      Assert::AreEqual(5.0f, distance, L"any hit at 6.5 with maxDistance=5 returns 5", LINE_INFO());
    }

    TEST_METHOD(naiveTraverserDiagonalRayHits) {
      auto fix = makeThreeBoxesOnXAxis();
      const NaiveKDTreeTraverser<FacetIntersection> traverser{};
      // diagonal toward middle box
      const auto dir = normalize3(Float4{1.0f, 0.0f, 0.0f, 0.0f});
      const Ray ray{Float4{-10.0f, 0.0f, 0.0f, 0.0f}, dir};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = traverser.findNearestIntersection(fix.nodesTraverser, *fix.root, rayCast, nullptr, intersection);

      Assert::IsTrue(distance < 100.0f, L"axial ray should hit a box", LINE_INFO());
      Assert::IsTrue(distance > 0.0f, L"hit must be in front of origin", LINE_INFO());
    }

    TEST_METHOD(naiveTraverserSelfOcclusionSkipsLastHit) {
      auto fix = makeThreeBoxesOnXAxis();
      const NaiveKDTreeTraverser<FacetIntersection> traverser{};
      const Ray ray{Float4{-10.0f, 0.0f, 0.0f, 0.0f}, Float4{1.0f, 0.0f, 0.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection priorHit{};

      const auto firstDistance = traverser.findNearestIntersection(fix.nodesTraverser, *fix.root, rayCast, nullptr, priorHit);
      Assert::AreEqual(6.5f, firstDistance, 0.001f, L"first hit at leftmost box", LINE_INFO());
      Assert::IsNotNull(priorHit.node, L"first cast should populate node pointer", LINE_INFO());

      FacetIntersection followUp{};
      const auto distance = traverser.findNearestIntersection(fix.nodesTraverser, *fix.root, rayCast, &priorHit, followUp);

      // skipping the leftmost box, the next nearest hit is the middle box at x=-0.5, distance 9.5
      Assert::AreEqual(9.5f, distance, 0.001f, L"self-occlusion advances to next box", LINE_INFO());
    }

    TEST_METHOD(naiveTraverserHitFromBehindCube) {
      auto fix = makeThreeBoxesOnXAxis();
      const NaiveKDTreeTraverser<FacetIntersection> traverser{};
      const Ray ray{Float4{10.0f, 0.0f, 0.0f, 0.0f}, Float4{-1.0f, 0.0f, 0.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = traverser.findNearestIntersection(fix.nodesTraverser, *fix.root, rayCast, nullptr, intersection);

      Assert::AreEqual(6.5f, distance, 0.001f, L"hit rightmost box from behind", LINE_INFO());
    }

    TEST_METHOD(naiveTraverserSetsIntersectionVertex) {
      auto fix = makeThreeBoxesOnXAxis();
      const NaiveKDTreeTraverser<FacetIntersection> traverser{};
      const Ray ray{Float4{-10.0f, 0.0f, 0.0f, 0.0f}, Float4{1.0f, 0.0f, 0.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      traverser.findNearestIntersection(fix.nodesTraverser, *fix.root, rayCast, nullptr, intersection);

      Assert::AreEqual(-3.5f, x(intersection.vertex), 0.001f, L"hit point x", LINE_INFO());
      Assert::IsNotNull(intersection.node, L"node pointer set", LINE_INFO());
    }

    TEST_METHOD(naiveTraverserManyBoxesAcrossSplitsAllReachable) {
      // 32 boxes laid out on x-axis; verify every box can be hit individually.
      BoxTreeFixture fix;
      const Float4 size{0.4f, 0.4f, 0.4f, 0.0f};
      for (int i = 0; i < 32; ++i) {
        const auto center = Float4{static_cast<Float>(i) * 1.0f, 0.0f, 0.0f, 0.0f};
        auto box = std::make_unique<Box>(center - size, center + size);
        fix.nodes.push_back(box.get());
        fix.ownedBoxes.push_back(std::move(box));
      }
      const MedianKDTreeBalancer balancer{};
      fix.root = balancer.build(fix.nodes);

      const NaiveKDTreeTraverser<FacetIntersection> traverser{};

      for (int i = 0; i < 32; ++i) {
        const auto cx = static_cast<Float>(i);
        const Ray ray{Float4{cx, -5.0f, 0.0f, 0.0f}, Float4{0.0f, 1.0f, 0.0f, 0.0f}};
        const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
        FacetIntersection intersection{};

        const auto distance = traverser.findNearestIntersection(fix.nodesTraverser, *fix.root, rayCast, nullptr, intersection);

        Assert::AreEqual(4.6f, distance, 0.001f, L"each box should be reachable through traversal", LINE_INFO());
      }
    }
  };
}
