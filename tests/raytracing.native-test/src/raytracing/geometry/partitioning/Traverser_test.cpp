#include "raytracing_tests.h"

#include "raytracing/geometry/partitioning/treebalancer/BruteForceSAHKDTreeBalancer.h"
#include "raytracing/geometry/partitioning/treebalancer/MedianKDTreeBalancer.h"
#include "raytracing/geometry/partitioning/treetraverser/NaiveKDTreeTraverser.h"
#include "raytracing/geometry/partitioning/treetraverser/VoxelizationKDTreeTraverser.h"
#include "raytracing/geometry/partitioning/treetraverser/WideKDTreeTraverser.h"

#include <array>
#include <bit>
#include <random>

namespace raytracer::test {
  // A minimal GeometryNodesTraverser over Box leaves: at each leaf, iterate the boxes and pick the
  // nearest hit. This lets us drive a NaiveKDTreeTraverser end-to-end without going through Mesh.
  class BoxNodesTraverser : public GeometryNodesTraverser<FacetIntersection> {
  public:
    Float findNearestIntersection(
        GeometryNodeSpan geometryNodes, const RayCast &rayCast, const FacetIntersection *const origin,
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
        GeometryNodeSpan geometryNodes, const RayCast &rayCast, const FacetIntersection *const origin,
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

  // Counts the leaves the traversal hands over, then intersects like BoxNodesTraverser.
  class CountingBoxNodesTraverser : public BoxNodesTraverser {
  public:
    mutable int leafCalls = 0;

    Float findNearestIntersection(
        GeometryNodeSpan geometryNodes, const RayCast &rayCast, const FacetIntersection *const origin,
        FacetIntersection &intersectionOut
    ) const override {
      ++leafCalls;
      return BoxNodesTraverser::findNearestIntersection(geometryNodes, rayCast, origin, intersectionOut);
    }

    Float findAnyIntersection(
        GeometryNodeSpan geometryNodes, const RayCast &rayCast, const FacetIntersection *const origin,
        FacetIntersection &intersectionOut
    ) const override {
      ++leafCalls;
      return BoxNodesTraverser::findAnyIntersection(geometryNodes, rayCast, origin, intersectionOut);
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

    static BoxTreeFixture makeBoxesAlongX(const int count) {
      BoxTreeFixture fix;
      const Float4 size{0.4f, 0.4f, 0.4f, 0.0f};
      for (int i = 0; i < count; ++i) {
        const auto center = Float4{static_cast<Float>(i) * 1.0f, 0.0f, 0.0f, 0.0f};
        auto box = std::make_unique<Box>(center - size, center + size);
        fix.nodes.push_back(box.get());
        fix.ownedBoxes.push_back(std::move(box));
      }
      const MedianKDTreeBalancer balancer{};
      fix.root = balancer.build(fix.nodes);
      return fix;
    }

    TEST_METHOD(naiveTraverserRayThroughBothChildrenFindsNearest) {
      // A ray along x crosses every split, so the traversal defers child B at each inner node and has to resume it.
      auto fix = makeBoxesAlongX(64);
      Assert::IsFalse(KDTreeRoot::isLeaf(fix.root->rootLink), L"64 boxes should split", LINE_INFO());
      const NaiveKDTreeTraverser<FacetIntersection> traverser{};
      FacetIntersection intersection{};

      const Ray fromLeft{Float4{-10.0f, 0.0f, 0.0f, 0.0f}, Float4{1.0f, 0.0f, 0.0f, 0.0f}};
      const RayCast leftCast{fromLeft, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      auto distance = traverser.findNearestIntersection(fix.nodesTraverser, *fix.root, leftCast, nullptr, intersection);
      Assert::AreEqual(9.6f, distance, 0.001f, L"first box from the left, at x=-0.4", LINE_INFO());
      Assert::IsTrue(intersection.node == fix.nodes.front(), L"hit node is the leftmost box", LINE_INFO());

      const Ray fromRight{Float4{70.0f, 0.0f, 0.0f, 0.0f}, Float4{-1.0f, 0.0f, 0.0f, 0.0f}};
      const RayCast rightCast{fromRight, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      distance = traverser.findNearestIntersection(fix.nodesTraverser, *fix.root, rightCast, nullptr, intersection);
      Assert::AreEqual(6.6f, distance, 0.001f, L"first box from the right, at x=63.4", LINE_INFO());
      Assert::IsTrue(intersection.node == fix.nodes.back(), L"hit node is the rightmost box", LINE_INFO());

      const RayCast shortCast{fromLeft, Zero<Float4::VectorBoolType>(), Size2(0, 0), 9.0f};
      distance = traverser.findNearestIntersection(fix.nodesTraverser, *fix.root, shortCast, nullptr, intersection);
      Assert::AreEqual(9.0f, distance, L"maxDistance ends before the first box", LINE_INFO());
    }

    TEST_METHOD(naiveTraverserSkipsSubtreeWhenBothChildBoxesAreMissed) {
      // Two clusters of small boxes (22 and 20), 20 units apart on x. The median of the 42 box minima falls into the
      // gap, so the median balancer splits the root there and each child box shrinks to its cluster. A ray crossing
      // the root box between the clusters misses both child boxes and must not reach any leaf.
      BoxTreeFixture fix;
      const Float4 size{0.02f, 0.02f, 0.02f, 0.0f};
      for (const auto &[offset, count] : {std::pair{0.0f, 22}, std::pair{20.0f, 20}}) {
        for (int i = 0; i < count; ++i) {
          const auto center = Float4{offset + static_cast<Float>(i) * 0.05f, 0.0f, 0.0f, 0.0f};
          auto box = std::make_unique<Box>(center - size, center + size);
          fix.nodes.push_back(box.get());
          fix.ownedBoxes.push_back(std::move(box));
        }
      }
      const MedianKDTreeBalancer balancer{};
      fix.root = balancer.build(fix.nodes);
      Assert::IsFalse(KDTreeRoot::isLeaf(fix.root->rootLink), L"42 boxes in two clusters should split", LINE_INFO());
      const auto &rootChildren = fix.root->childBoundings[fix.root->rootLink];
      Assert::IsTrue(x(rootChildren.boundingA.maximum) < 10.0f, L"child A shrinks to the left cluster", LINE_INFO());
      Assert::IsTrue(x(rootChildren.boundingB.minimum) > 10.0f, L"child B shrinks to the right cluster", LINE_INFO());

      const NaiveKDTreeTraverser<FacetIntersection> traverser{};
      const CountingBoxNodesTraverser counting{};
      const Ray betweenClusters{Float4{10.0f, -5.0f, 0.0f, 0.0f}, Float4{0.0f, 1.0f, 0.0f, 0.0f}};
      const RayCast rayCast{betweenClusters, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      Assert::IsTrue(overlaps(rayCast, fix.root->rootBounding), L"the ray crosses the root box", LINE_INFO());
      FacetIntersection intersection{};

      auto distance = traverser.findNearestIntersection(counting, *fix.root, rayCast, nullptr, intersection);
      Assert::AreEqual(100.0f, distance, L"nearest: nothing between the clusters", LINE_INFO());
      Assert::AreEqual(0, counting.leafCalls, L"nearest: no leaf reached", LINE_INFO());

      distance = traverser.findAnyIntersection(counting, *fix.root, rayCast, nullptr, intersection);
      Assert::AreEqual(100.0f, distance, L"any: nothing between the clusters", LINE_INFO());
      Assert::AreEqual(0, counting.leafCalls, L"any: no leaf reached", LINE_INFO());
    }

    TEST_METHOD(naiveTraverserNearestEntersNearerChildFirst) {
      // The two clusters of naiveTraverserSkipsSubtreeWhenBothChildBoxesAreMissed, each an inner node with two leaves.
      // A ray from +x along the boxes enters child B first. Taking B first, its hit shortens the ray, and A's leaf
      // boxes then lie beyond it: only B's two leaves are called. Taking A first would also call A's two leaves.
      BoxTreeFixture fix;
      const Float4 size{0.02f, 0.02f, 0.02f, 0.0f};
      for (const auto &[offset, count] : {std::pair{0.0f, 22}, std::pair{20.0f, 20}}) {
        for (int i = 0; i < count; ++i) {
          const auto center = Float4{offset + static_cast<Float>(i) * 0.05f, 0.0f, 0.0f, 0.0f};
          auto box = std::make_unique<Box>(center - size, center + size);
          fix.nodes.push_back(box.get());
          fix.ownedBoxes.push_back(std::move(box));
        }
      }
      const MedianKDTreeBalancer balancer{};
      fix.root = balancer.build(fix.nodes);
      Assert::IsFalse(KDTreeRoot::isLeaf(fix.root->rootLink), L"42 boxes in two clusters should split", LINE_INFO());
      const auto &rootChildren = fix.root->childLinks[fix.root->rootLink];
      Assert::IsFalse(KDTreeRoot::isLeaf(rootChildren.childA), L"child A (22 boxes) is inner", LINE_INFO());
      Assert::IsFalse(KDTreeRoot::isLeaf(rootChildren.childB), L"child B (20 boxes) is inner", LINE_INFO());

      const NaiveKDTreeTraverser<FacetIntersection> traverser{};
      FacetIntersection intersection{};

      // the last box of the right cluster ends at x = 20.95 + 0.02
      const Ray fromRight{Float4{30.0f, 0.0f, 0.0f, 0.0f}, Float4{-1.0f, 0.0f, 0.0f, 0.0f}};
      const RayCast rightCast{fromRight, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      const CountingBoxNodesTraverser rightCounting{};
      const auto rightDistance =
          traverser.findNearestIntersection(rightCounting, *fix.root, rightCast, nullptr, intersection);
      Assert::AreEqual(9.03f, rightDistance, 0.001f, L"from +x: nearest box of the right cluster", LINE_INFO());
      Assert::AreEqual(2, rightCounting.leafCalls, L"from +x: only child B's leaves", LINE_INFO());

      // the first box of the left cluster starts at x = -0.02; child A is nearer and comes first anyway
      const Ray fromLeft{Float4{-10.0f, 0.0f, 0.0f, 0.0f}, Float4{1.0f, 0.0f, 0.0f, 0.0f}};
      const RayCast leftCast{fromLeft, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      const CountingBoxNodesTraverser leftCounting{};
      const auto leftDistance =
          traverser.findNearestIntersection(leftCounting, *fix.root, leftCast, nullptr, intersection);
      Assert::AreEqual(9.98f, leftDistance, 0.001f, L"from -x: nearest box of the left cluster", LINE_INFO());
      Assert::AreEqual(2, leftCounting.leafCalls, L"from -x: only child A's leaves", LINE_INFO());
    }

    TEST_METHOD(wideTraverserMatchesNaive) {
      // The wide traverser against the naive one on random box trees from both balancers: a single-leaf root (3
      // boxes) and irregular trees with 2-, 3- and 4-lane wide nodes. Random and axis-parallel rays (infinite
      // reciprocals, and NaN slabs where the origin lies on a box face), with a long and a short reach. Nearest: same
      // distance and box. Any-hit: same outcome and, as the leaves come in the same order, the same box.
      const NaiveKDTreeTraverser<FacetIntersection> naive{};
      const WideKDTreeTraverser<FacetIntersection> wide{};
      auto generator = std::mt19937{0x5eed1234u};
      const auto uniform = [&generator](const Float low, const Float high) {
        return std::uniform_real_distribution<Float>(low, high)(generator);
      };

      std::array<int, 5> wideNodesByLanes{};
      auto nearestHits = 0;
      auto anyHits = 0;
      for (const auto boxCount : {3, 24, 80, 300}) {
        for (const auto sah : {false, true}) {
          BoxTreeFixture fix;
          for (int i = 0; i < boxCount; ++i) {
            const auto center = Float4{uniform(-5.0f, 5.0f), uniform(-5.0f, 5.0f), uniform(-5.0f, 5.0f), 0.0f};
            const auto half = Float4{uniform(0.05f, 0.6f), uniform(0.05f, 0.6f), uniform(0.05f, 0.6f), 0.0f};
            auto box = std::make_unique<Box>(center - half, center + half);
            fix.nodes.push_back(box.get());
            fix.ownedBoxes.push_back(std::move(box));
          }
          fix.root = sah ? BruteForceSAHKDTreeBalancer{}.build(fix.nodes) : MedianKDTreeBalancer{}.build(fix.nodes);
          if (boxCount == 3) {
            Assert::IsTrue(KDTreeRoot::isLeaf(fix.root->wideRootLink), L"3 boxes: single-leaf root", LINE_INFO());
          }
          for (const auto &node : fix.root->wideNodes) {
            ++wideNodesByLanes[std::popcount(node.lanes)];
          }

          const auto faceMinimum = fix.ownedBoxes.front()->getBounding().minimum;
          for (int i = 0; i < 400; ++i) {
            auto origin = Float4{uniform(-8.0f, 8.0f), uniform(-8.0f, 8.0f), uniform(-8.0f, 8.0f), 1.0f};
            auto direction = Float4{uniform(-1.0f, 1.0f), uniform(-1.0f, 1.0f), uniform(-1.0f, 1.0f), 0.0f};
            if (i % 4 == 1 || i % 4 == 3) {
              // aimed at a box, so that enough rays hit
              const auto &target = *fix.ownedBoxes[static_cast<std::size_t>(i) % fix.ownedBoxes.size()];
              direction = center(target.getBounding()) - origin;
              direction[3] = 0.0f;
            }
            if (i % 4 == 0) {
              const auto sign = i % 8 == 0 ? 1.0f : -1.0f;
              const auto axis = (i / 4) % 3;
              direction = Float4{axis == 0 ? sign : 0.0f, axis == 1 ? sign : 0.0f, axis == 2 ? sign : 0.0f, 0.0f};
              if (i % 16 == 4) {
                // on the first box's minimum face along the next axis: 0 * inf = NaN in that slab
                const auto other = (axis + 1) % 3;
                origin[other] = faceMinimum[other];
              }
            }
            const Ray ray{origin, normalize3(direction)};
            const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), i % 2 == 0 ? 100.0f : 6.0f};

            FacetIntersection naiveHit{};
            FacetIntersection wideHit{};
            const auto naiveNearest =
                naive.findNearestIntersection(fix.nodesTraverser, *fix.root, rayCast, nullptr, naiveHit);
            const auto wideNearest =
                wide.findNearestIntersection(fix.nodesTraverser, *fix.root, rayCast, nullptr, wideHit);
            Assert::AreEqual(naiveNearest, wideNearest, L"nearest distance", LINE_INFO());
            if (naiveNearest < rayCast.maxDistance) {
              Assert::IsTrue(naiveHit.node == wideHit.node, L"nearest box", LINE_INFO());
              ++nearestHits;
            }

            FacetIntersection naiveAny{};
            FacetIntersection wideAny{};
            const auto naiveAnyDistance =
                naive.findAnyIntersection(fix.nodesTraverser, *fix.root, rayCast, nullptr, naiveAny);
            const auto wideAnyDistance =
                wide.findAnyIntersection(fix.nodesTraverser, *fix.root, rayCast, nullptr, wideAny);
            Assert::AreEqual(naiveAnyDistance, wideAnyDistance, L"any-hit distance", LINE_INFO());
            if (naiveAnyDistance < rayCast.maxDistance) {
              Assert::IsTrue(naiveAny.node == wideAny.node, L"any-hit box", LINE_INFO());
              ++anyHits;
            }
          }
        }
      }
      Assert::IsTrue(wideNodesByLanes[2] > 0, L"wide nodes with 2 lanes", LINE_INFO());
      Assert::IsTrue(wideNodesByLanes[3] > 0, L"wide nodes with 3 lanes", LINE_INFO());
      Assert::IsTrue(wideNodesByLanes[4] > 0, L"wide nodes with 4 lanes", LINE_INFO());
      Assert::IsTrue(nearestHits > 500 && anyHits > 500, L"enough hits to compare", LINE_INFO());
    }

    TEST_METHOD(naiveTraverserAnyHitThroughBothChildren) {

      auto fix = makeBoxesAlongX(64);
      const NaiveKDTreeTraverser<FacetIntersection> traverser{};
      FacetIntersection intersection{};

      const Ray alongX{Float4{-10.0f, 0.0f, 0.0f, 0.0f}, Float4{1.0f, 0.0f, 0.0f, 0.0f}};
      const RayCast hitting{alongX, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      const auto distance =
          traverser.findAnyIntersection(fix.nodesTraverser, *fix.root, hitting, nullptr, intersection);
      Assert::IsTrue(distance < 100.0f, L"some box is hit", LINE_INFO());
      Assert::IsNotNull(intersection.node, L"hit node set", LINE_INFO());

      const Ray between{Float4{-10.0f, 0.45f, 0.0f, 0.0f}, Float4{1.0f, 0.0f, 0.0f, 0.0f}};
      const RayCast missing{between, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      const auto missDistance =
          traverser.findAnyIntersection(fix.nodesTraverser, *fix.root, missing, nullptr, intersection);
      Assert::AreEqual(100.0f, missDistance, L"ray just above all boxes misses", LINE_INFO());
    }

    TEST_METHOD(voxelizationTraverserReportsLeafBoundings) {
      // The voxelization traverser is a debug view: it intersects the leaf boundings, not the geometry, and sets no
      // node. 64 boxes force inner nodes, three boxes keep the whole tree a single leaf.
      BoxTreeFixture split;
      const Float4 size{0.4f, 0.4f, 0.4f, 0.0f};
      for (int i = 0; i < 64; ++i) {
        const auto center = Float4{static_cast<Float>(i) * 1.0f, 0.0f, 0.0f, 0.0f};
        auto box = std::make_unique<Box>(center - size, center + size);
        split.nodes.push_back(box.get());
        split.ownedBoxes.push_back(std::move(box));
      }
      const MedianKDTreeBalancer balancer{};
      split.root = balancer.build(split.nodes);
      Assert::IsFalse(KDTreeRoot::isLeaf(split.root->rootLink), L"64 boxes should split", LINE_INFO());

      const VoxelizationKDTreeTraverser<FacetIntersection> traverser{};
      const Ray upwards{Float4{0.0f, -5.0f, 0.0f, 0.0f}, Float4{0.0f, 1.0f, 0.0f, 0.0f}};
      const RayCast hitting{upwards, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};
      intersection.node = split.nodes.front();

      auto distance =
          traverser.findNearestIntersection(split.nodesTraverser, *split.root, hitting, nullptr, intersection);
      Assert::AreEqual(4.6f, distance, 0.001f, L"enters the leaf bounding at y=-0.4", LINE_INFO());
      Assert::IsNull(intersection.node, L"no geometry node for a leaf bounding", LINE_INFO());

      distance = traverser.findAnyIntersection(split.nodesTraverser, *split.root, hitting, nullptr, intersection);
      Assert::AreEqual(4.6f, distance, 0.001f, L"any hit also reports the leaf bounding", LINE_INFO());

      const Ray above{Float4{0.0f, 5.0f, 0.0f, 0.0f}, Float4{1.0f, 0.0f, 0.0f, 0.0f}};
      const RayCast missing{above, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      distance = traverser.findNearestIntersection(split.nodesTraverser, *split.root, missing, nullptr, intersection);
      Assert::AreEqual(100.0f, distance, L"miss returns maxDistance", LINE_INFO());

      auto single = makeThreeBoxesOnXAxis();
      Assert::IsTrue(KDTreeRoot::isLeaf(single.root->rootLink), L"three boxes stay a single leaf", LINE_INFO());
      const Ray alongX{Float4{-10.0f, 0.0f, 0.0f, 0.0f}, Float4{1.0f, 0.0f, 0.0f, 0.0f}};
      const RayCast throughRoot{alongX, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      distance =
          traverser.findNearestIntersection(single.nodesTraverser, *single.root, throughRoot, nullptr, intersection);
      Assert::AreEqual(6.5f, distance, 0.001f, L"enters the root leaf bounding at x=-3.5", LINE_INFO());
    }
  };
}
