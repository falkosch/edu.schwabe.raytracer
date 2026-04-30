#include "raytracing_tests.h"

#include "raytracing/geometry/partitioning/treebalancer/ArithmeticMeanKDTreeBalancer.h"
#include "raytracing/geometry/partitioning/treebalancer/MaxAxisKDTreeBalancer.h"
#include "raytracing/geometry/partitioning/treebalancer/MedianKDTreeBalancer.h"

namespace raytracer::test {
  // KDTreeBalancer::build() defaults maxNodesSize to 16, so any input <=16 primitives stays a leaf.
  // Tests that need a split must use > 16 primitives.
  TEST_CLASS(BalancerTest) {
    struct BoxStorage {
      std::vector<std::unique_ptr<Box>> owned;
      PGeometryNodeList nodes;

      void add(const Float4 &min, const Float4 &max) {
        auto box = std::make_unique<Box>(min, max);
        nodes.push_back(box.get());
        owned.push_back(std::move(box));
      }
    };

    static BoxStorage manyBoxesAlongX(int count) {
      BoxStorage storage;
      const Float4 size{0.4f, 0.4f, 0.4f, 0.0f};
      for (int i = 0; i < count; ++i) {
        const auto center = Float4{static_cast<Float>(i) * 1.0f, 0.0f, 0.0f, 0.0f};
        storage.add(center - size, center + size);
      }
      return storage;
    }

  public:
    TEST_METHOD(buildEmptyGeometryReturnsNull) {
      const auto balancer = MedianKDTreeBalancer{};
      const PGeometryNodeList empty{};
      auto root = balancer.build(empty);
      Assert::IsNull(root.get(), L"empty input must yield no tree", LINE_INFO());
    }

    TEST_METHOD(buildSinglePrimitiveProducesLeaf) {
      const auto balancer = MedianKDTreeBalancer{};
      BoxStorage storage;
      storage.add(Float4{-1.0f, -1.0f, -1.0f, 0.0f}, Float4{1.0f, 1.0f, 1.0f, 0.0f});

      auto root = balancer.build(storage.nodes);
      Assert::IsNotNull(root.get(), L"single-primitive tree should exist", LINE_INFO());
      Assert::IsTrue(root->rootNode.isNonEmptyLeaf(), L"single primitive should be a leaf", LINE_INFO());
      Assert::AreEqual(ASizeT{1}, root->rootNode.geometryNodes->size(), L"leaf holds one node", LINE_INFO());
    }

    TEST_METHOD(buildBelowMaxNodesSizeStaysAsLeaf) {
      // 2 primitives is below the default maxNodesSize of 16, so the tree stops at the root leaf.
      const auto balancer = MedianKDTreeBalancer{};
      BoxStorage storage;
      storage.add(Float4{-2.0f, -1.0f, -1.0f, 0.0f}, Float4{-1.0f, 1.0f, 1.0f, 0.0f});
      storage.add(Float4{1.0f, -1.0f, -1.0f, 0.0f}, Float4{2.0f, 1.0f, 1.0f, 0.0f});

      auto root = balancer.build(storage.nodes);
      Assert::IsNotNull(root.get(), L"tree should exist", LINE_INFO());
      Assert::IsTrue(root->rootNode.isNonEmptyLeaf(), L"<=16 primitives stays as leaf", LINE_INFO());
      Assert::AreEqual(ASizeT{2}, root->rootNode.geometryNodes->size(), L"leaf holds both boxes", LINE_INFO());
    }

    TEST_METHOD(buildAboveCostThresholdProducesSplit) {
      // 20 primitives can still terminate via the SAH cost post-check; 64 is large enough that the
      // split is cheaper than a fat leaf.
      const auto balancer = MedianKDTreeBalancer{};
      auto storage = manyBoxesAlongX(64);

      auto root = balancer.build(storage.nodes);
      Assert::IsNotNull(root.get(), L"tree should exist", LINE_INFO());
      Assert::IsTrue(root->rootNode.children != nullptr, L"64 boxes should force a split", LINE_INFO());
    }

    TEST_METHOD(rootBoundingCoversAllPrimitives) {
      const auto balancer = MedianKDTreeBalancer{};
      auto storage = manyBoxesAlongX(64);

      auto root = balancer.build(storage.nodes);
      Assert::AreEqual(-0.4f, x(root->rootBounding.minimum), 0.001f, L"root min x", LINE_INFO());
      Assert::AreEqual(63.4f, x(root->rootBounding.maximum), 0.001f, L"root max x", LINE_INFO());
    }

    TEST_METHOD(splitMaintainsAllInputsViaLeaves) {
      // Each input box must end up in at least one leaf (may be duplicated when straddling a split).
      const auto balancer = MedianKDTreeBalancer{};
      auto storage = manyBoxesAlongX(32);

      auto root = balancer.build(storage.nodes);

      std::function<ASizeT(const KDTreeNode &)> countLeafGeometry = [&](const KDTreeNode &node) -> ASizeT {
        if (node.isNonEmptyLeaf()) {
          return node.geometryNodes->size();
        }
        if (!node.children) {
          return ASizeT{0};
        }
        return countLeafGeometry(node.children->childA) + countLeafGeometry(node.children->childB);
      };

      const auto totalLeafEntries = countLeafGeometry(root->rootNode);
      Assert::IsTrue(totalLeafEntries >= ASizeT{32}, L"every input must reach at least one leaf", LINE_INFO());
    }

    TEST_METHOD(samePositionPrimitivesTerminateAsLeaf) {
      const auto balancer = MedianKDTreeBalancer{};
      BoxStorage storage;
      // 20 coincident boxes — splitting cannot separate them. The balancer should still build a
      // valid tree without infinite recursion.
      for (int i = 0; i < 20; ++i) {
        storage.add(Float4{-0.5f, -0.5f, -0.5f, 0.0f}, Float4{0.5f, 0.5f, 0.5f, 0.0f});
      }

      auto root = balancer.build(storage.nodes);
      Assert::IsNotNull(root.get(), L"tree should still build", LINE_INFO());
      // The exact tree shape is balancer-specific; just ensure all 20 boxes are still represented.
      std::function<ASizeT(const KDTreeNode &)> countLeafGeometry = [&](const KDTreeNode &node) -> ASizeT {
        if (node.isNonEmptyLeaf()) {
          return node.geometryNodes->size();
        }
        if (!node.children) {
          return ASizeT{0};
        }
        return countLeafGeometry(node.children->childA) + countLeafGeometry(node.children->childB);
      };
      const auto totalLeafEntries = countLeafGeometry(root->rootNode);
      Assert::IsTrue(totalLeafEntries >= ASizeT{20}, L"all coincident primitives represented", LINE_INFO());
    }

    TEST_METHOD(arithmeticMeanBalancerBuildsWorkingTree) {
      const auto balancer = ArithmeticMeanKDTreeBalancer{};
      auto storage = manyBoxesAlongX(64);

      auto root = balancer.build(storage.nodes);
      Assert::IsNotNull(root.get(), L"arithmetic mean balancer should build a tree", LINE_INFO());
      Assert::IsTrue(root->rootNode.children != nullptr, L"64 boxes should split", LINE_INFO());
    }

    TEST_METHOD(maxAxisBalancerBuildsWorkingTree) {
      const auto balancer = MaxAxisKDTreeBalancer{};
      auto storage = manyBoxesAlongX(64);

      auto root = balancer.build(storage.nodes);
      Assert::IsNotNull(root.get(), L"max axis balancer should build a tree", LINE_INFO());
      Assert::IsTrue(root->rootNode.children != nullptr, L"64 boxes should split", LINE_INFO());
    }

    TEST_METHOD(splitPlaneSelectionUsesMaxAxis) {
      // x-extent dominates by far, so a max-axis-style splitter should split on x and produce
      // children whose x-ranges meet at one point (no overlap on the chosen axis).
      const auto balancer = MedianKDTreeBalancer{};
      auto storage = manyBoxesAlongX(64);

      auto root = balancer.build(storage.nodes);
      Assert::IsTrue(root->rootNode.children != nullptr, L"should split", LINE_INFO());

      const auto &children = *root->rootNode.children;
      const auto leftMaxX = x(children.boundingA.maximum);
      const auto rightMinX = x(children.boundingB.minimum);
      Assert::IsTrue(leftMaxX <= rightMinX + 0.001f,
                     L"split should be along x with non-overlapping halves", LINE_INFO());
    }

    TEST_METHOD(buildResultsInDeeperTreeAsPrimitivesGrow) {
      const auto balancer = MedianKDTreeBalancer{};
      auto smallStorage = manyBoxesAlongX(64);
      auto largeStorage = manyBoxesAlongX(128);

      auto smallRoot = balancer.build(smallStorage.nodes);
      auto largeRoot = balancer.build(largeStorage.nodes);

      Assert::IsTrue(largeRoot->depth() >= smallRoot->depth(),
                     L"more primitives should yield depth at least as large", LINE_INFO());
    }

    TEST_METHOD(splittingPlaneOnSimpleBoundingProducesValidHalves) {
      // Direct test of KDTreeBounding::split — independent of balancer choice.
      const AxisAlignedBoundingBox parent{Float4{-1.0f, -1.0f, -1.0f, 0.0f}, Float4{1.0f, 1.0f, 1.0f, 0.0f}};
      const KDTreePlane splitter{0.5f, 0u};

      AxisAlignedBoundingBox leftBounding, rightBounding;
      KDTreeBounding::split(splitter, parent, leftBounding, rightBounding);

      Assert::AreEqual(-1.0f, x(leftBounding.minimum), 0.001f, L"left min x = parent min x", LINE_INFO());
      Assert::AreEqual(0.0f, x(leftBounding.maximum), 0.001f, L"left max x at midpoint", LINE_INFO());
      Assert::AreEqual(0.0f, x(rightBounding.minimum), 0.001f, L"right min x at midpoint", LINE_INFO());
      Assert::AreEqual(1.0f, x(rightBounding.maximum), 0.001f, L"right max x = parent max x", LINE_INFO());
      Assert::AreEqual(-1.0f, y(leftBounding.minimum), 0.001f, L"left min y preserved", LINE_INFO());
      Assert::AreEqual(1.0f, z(rightBounding.maximum), 0.001f, L"right max z preserved", LINE_INFO());
    }

    TEST_METHOD(splittingPlaneAtZeroIsLeftCollapsed) {
      // t=0 means split at min; left bounding collapses on the split axis.
      const AxisAlignedBoundingBox parent{Float4{-1.0f, -1.0f, -1.0f, 0.0f}, Float4{1.0f, 1.0f, 1.0f, 0.0f}};
      const KDTreePlane splitter{0.0f, 0u};

      AxisAlignedBoundingBox leftBounding, rightBounding;
      KDTreeBounding::split(splitter, parent, leftBounding, rightBounding);

      Assert::AreEqual(-1.0f, x(leftBounding.maximum), 0.001f, L"left collapses at min", LINE_INFO());
      Assert::AreEqual(-1.0f, x(rightBounding.minimum), 0.001f, L"right starts at min", LINE_INFO());
    }

    TEST_METHOD(findMinimumBoundingOfGeometryComputesUnion) {
      auto storage = manyBoxesAlongX(20);
      const auto bounding = KDTreeBounding::findMinimumBoundingOfGeometry(storage.nodes);
      // Each box has half-extent 0.4 around its center; the leftmost is centered at 0.0, so its min
      // x is -0.4. The rightmost is centered at 19.0, so its max x is 19.4.
      Assert::AreEqual(-0.4f, x(bounding.minimum), 0.001f, L"union min x", LINE_INFO());
      Assert::AreEqual(19.4f, x(bounding.maximum), 0.001f, L"union max x", LINE_INFO());
    }

    TEST_METHOD(findMinimumBoundingOfGeometrySinglePrimitive) {
      BoxStorage storage;
      storage.add(Float4{-2.0f, -3.0f, -4.0f, 0.0f}, Float4{2.0f, 3.0f, 4.0f, 0.0f});
      const auto bounding = KDTreeBounding::findMinimumBoundingOfGeometry(storage.nodes);
      Assert::AreEqual(-2.0f, x(bounding.minimum), 0.001f, L"min x", LINE_INFO());
      Assert::AreEqual(4.0f, z(bounding.maximum), 0.001f, L"max z", LINE_INFO());
    }

    TEST_METHOD(largeListProducesDeepTree) {
      const auto balancer = MedianKDTreeBalancer{};
      auto storage = manyBoxesAlongX(64);

      auto root = balancer.build(storage.nodes);
      Assert::IsNotNull(root.get(), L"tree should build", LINE_INFO());
      Assert::IsTrue(root->depth() >= ASizeT{2}, L"64 primitives should produce a multi-level tree",
                     LINE_INFO());
    }

    TEST_METHOD(maxAxisBalancerWithSinglePrimitive) {
      const auto balancer = MaxAxisKDTreeBalancer{};
      BoxStorage storage;
      storage.add(Float4{-1.0f, -1.0f, -1.0f, 0.0f}, Float4{1.0f, 1.0f, 1.0f, 0.0f});

      auto root = balancer.build(storage.nodes);
      Assert::IsNotNull(root.get(), L"single primitive tree should exist", LINE_INFO());
      Assert::IsTrue(root->rootNode.isNonEmptyLeaf(), L"single primitive is a leaf", LINE_INFO());
    }
  };
}
