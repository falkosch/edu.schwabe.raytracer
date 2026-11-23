#include "raytracing_tests.h"

#include "../../../common/TempFile.h"

#include "raytracing/geometry/forms/meshes/Mesh.h"
#include "raytracing/geometry/forms/meshes/MeshGeometryNode.h"
#include "raytracing/geometry/partitioning/treebalancer/BruteForceSAHKDTreeBalancer.h"
#include "raytracing/geometry/partitioning/treebalancer/MedianKDTreeBalancer.h"
#include "raytracing/geometry/partitioning/treetraverser/NaiveKDTreeTraverser.h"
#include "raytracing/geometry/partitioning/treetraverser/WideKDTreeTraverser.h"

#include <fstream>
#include <random>
#include <utility>
#include <vector>

namespace raytracer::test {
  namespace {
    // Counts the queries the mesh hands to its traverser, then traverses with the wide traverser
    class CountingKDTreeTraverser : public KDTreeTraverser<FacetIntersection> {
    public:
      mutable int queries = 0;

      Float findNearestIntersection(
          const GeometryNodesTraverser<FacetIntersection> &geometryNodesTraverser, const KDTreeRoot &root,
          const RayCast &rayCast, const FacetIntersection *const originIntersection,
          FacetIntersection &intersectionOut
      ) const override {
        ++queries;
        return traverser.findNearestIntersection(
            geometryNodesTraverser, root, rayCast, originIntersection, intersectionOut
        );
      }

      Float findAnyIntersection(
          const GeometryNodesTraverser<FacetIntersection> &geometryNodesTraverser, const KDTreeRoot &root,
          const RayCast &rayCast, const FacetIntersection *const originIntersection,
          FacetIntersection &intersectionOut
      ) const override {
        ++queries;
        return traverser.findAnyIntersection(
            geometryNodesTraverser, root, rayCast, originIntersection, intersectionOut
        );
      }

    private:
      WideKDTreeTraverser<FacetIntersection> traverser;
    };

    ASizeT facetIndexOf(const FacetIntersection &intersection) {
      const auto *node = dynamic_cast<const MeshGeometryNode *>(intersection.node);
      Assert::IsNotNull(node, L"hit a mesh facet", LINE_INFO());
      return node->index;
    }

    // Rays from outside towards points in the cube and from inside the cube in any direction
    std::vector<Ray> cubeRays() {
      auto generator = std::mt19937{0x5eed1234u};
      auto uniform = std::uniform_real_distribution<Float>(-1.0f, 1.0f);
      std::vector<Ray> rays;
      for (auto i = 0; i < 32; ++i) {
        const auto origin = Float4{3.0f * uniform(generator), 3.0f * uniform(generator), -3.0f, 1.0f};
        const auto target =
          Float4{0.5f * uniform(generator), 0.5f * uniform(generator), 0.5f * uniform(generator), 1.0f};
        rays.emplace_back(origin, normalize3(target - origin));
        const auto inside =
          Float4{0.4f * uniform(generator), 0.4f * uniform(generator), 0.4f * uniform(generator), 1.0f};
        const auto direction = Float4{uniform(generator), uniform(generator), uniform(generator), 0.0f};
        rays.emplace_back(inside, normalize3(direction));
      }
      return rays;
    }

    // Every ray in every culling mode, also continued from its hit facet as a secondary ray, must hit the same facet
    // of both meshes at the same distance. Returns the count of nearest hits.
    int assertSameIntersections(const Mesh &expected, const Mesh &actual) {
      // relative to distances of up to about 6; the KD-tree's leaves take the block test, whose FMA chains differ in
      // the last bits
      constexpr auto DistanceTolerance = 1e-5f;
      auto hits = 0;
      for (const auto culling : {-1, 0, 1}) {
        for (const auto &ray : cubeRays()) {
          const RayCast rayCast{ray, cullingOrientationToMask(culling), Size2(0, 0), 100.0f};
          auto expectedHit = FacetIntersection{};
          auto actualHit = FacetIntersection{};
          const auto expectedDistance = expected.findNearestIntersection(rayCast, nullptr, expectedHit);
          const auto actualDistance = actual.findNearestIntersection(rayCast, nullptr, actualHit);
          Assert::AreEqual(expectedDistance, actualDistance, DistanceTolerance, L"nearest distance", LINE_INFO());

          auto expectedAny = FacetIntersection{};
          auto actualAny = FacetIntersection{};
          Assert::AreEqual(
              expected.findAnyIntersection(rayCast, nullptr, expectedAny) < rayCast.maxDistance,
              actual.findAnyIntersection(rayCast, nullptr, actualAny) < rayCast.maxDistance, L"any hit", LINE_INFO()
          );
          if (expectedDistance >= rayCast.maxDistance) {
            continue;
          }
          ++hits;
          Assert::AreEqual(facetIndexOf(expectedHit), facetIndexOf(actualHit), L"nearest facet", LINE_INFO());

          // the secondary ray from the hit facet must skip it
          const RayCast continued{
              Ray{ray.origin + ray.direction * Float4(expectedDistance), ray.direction},
              cullingOrientationToMask(culling), Size2(0, 0), 100.0f
          };
          auto expectedNext = FacetIntersection{};
          auto actualNext = FacetIntersection{};
          const auto expectedNextDistance = expected.findNearestIntersection(continued, &expectedHit, expectedNext);
          const auto actualNextDistance = actual.findNearestIntersection(continued, &actualHit, actualNext);
          Assert::AreEqual(
              expectedNextDistance, actualNextDistance, DistanceTolerance, L"secondary distance", LINE_INFO()
          );
          if (expectedNextDistance < continued.maxDistance) {
            Assert::AreEqual(facetIndexOf(expectedNext), facetIndexOf(actualNext), L"secondary facet", LINE_INFO());
          }
        }
      }
      return hits;
    }
  }

  TEST_CLASS(MeshTest) {
    // Mesh::buildXxxMesh static helpers without a traverser and a balancer build a mesh without a
    // KD-tree, so findNearestIntersection takes the brute-force fallback over the geometryNodes list.
    // Given both, they wire the KD-tree as loadFromOffFile does. Note: setupMesh() rescales/translates
    // the input vertices via computeStandardMesh() — the input shape is centered at the origin and
    // uniformly scaled to fit within [-0.5, 0.5] along the axes with non-zero extent.
    static std::unique_ptr<Mesh> makeTriangleMesh() {
      return Mesh::buildTriangleMesh();
    }

    static std::unique_ptr<Mesh> makeCubeMesh() {
      return Mesh::buildCubeMesh();
    }

  public:
    TEST_METHOD(buildTriangleMeshHasOneFacet) {
      auto mesh = makeTriangleMesh();
      Assert::AreEqual(ASizeT{1}, mesh->getFaceCount(), L"triangle mesh face count", LINE_INFO());
    }

    TEST_METHOD(buildCubeMeshHasTwelveFacets) {
      auto mesh = makeCubeMesh();
      Assert::AreEqual(ASizeT{12}, mesh->getFaceCount(), L"cube mesh face count", LINE_INFO());
    }

    TEST_METHOD(triangleMeshBoundingIsRecenteredAndScaled) {
      // Input triangle (-0.5,0,0), (0,1,0), (0.5,0,0) has bounds [-0.5, 0]..[0.5, 1] (z-extent 0).
      // computeStandardMesh recenters at origin and uniformly scales by 1/min(extent) over non-zero
      // axes; here both x and y extents are 1 so scale is 1. Result bounding is [-0.5, -0.5, 0]..
      // [0.5, 0.5, 0].
      auto mesh = makeTriangleMesh();
      const auto box = mesh->getBounding();
      Assert::AreEqual(-0.5f, x(box.minimum), 0.001f, L"min x", LINE_INFO());
      Assert::AreEqual(-0.5f, y(box.minimum), 0.001f, L"min y", LINE_INFO());
      Assert::AreEqual(0.5f, x(box.maximum), 0.001f, L"max x", LINE_INFO());
      Assert::AreEqual(0.5f, y(box.maximum), 0.001f, L"max y", LINE_INFO());
    }

    TEST_METHOD(cubeMeshBoundingIsRecenteredAndScaled) {
      // Input cube spans [-1, 1]^3. After computeStandardMesh: scale 1/2, centered → [-0.5, 0.5]^3.
      auto mesh = makeCubeMesh();
      const auto box = mesh->getBounding();
      Assert::AreEqual(-0.5f, x(box.minimum), 0.001f, L"min x", LINE_INFO());
      Assert::AreEqual(-0.5f, y(box.minimum), 0.001f, L"min y", LINE_INFO());
      Assert::AreEqual(-0.5f, z(box.minimum), 0.001f, L"min z", LINE_INFO());
      Assert::AreEqual(0.5f, x(box.maximum), 0.001f, L"max x", LINE_INFO());
      Assert::AreEqual(0.5f, y(box.maximum), 0.001f, L"max y", LINE_INFO());
      Assert::AreEqual(0.5f, z(box.maximum), 0.001f, L"max z", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionTriangleHit) {
      // Rescaled triangle vertices: (-0.5, -0.5, 0), (0, 0.5, 0), (0.5, -0.5, 0). y=-0.25 is inside.
      auto mesh = makeTriangleMesh();
      const Ray ray{Float4{0.0f, -0.25f, -5.0f, 1.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = mesh->findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(5.0f, distance, 0.001f, L"hit triangle plane at z=0", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionTriangleMiss) {
      auto mesh = makeTriangleMesh();
      const Ray ray{Float4{0.0f, 5.0f, -5.0f, 1.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = mesh->findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(100.0f, distance, L"miss should return maxDistance", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionTriangleMissAboveSlope) {
      // Far corner of bounding box that is outside the triangle: (0.4, 0.4) — above and right of
      // the right slope edge.
      auto mesh = makeTriangleMesh();
      const Ray ray{Float4{0.4f, 0.4f, -5.0f, 1.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = mesh->findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(100.0f, distance, L"corner outside triangle should miss", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionCubeFromOutside) {
      // Rescaled cube spans [-0.5, 0.5]^3. Ray from z=-5 hits -z face at z=-0.5 (distance 4.5).
      // Offset ray.x from the diagonal edge between the two -z-face triangles: Havel rejects hits
      // on the shared edge.
      auto mesh = makeCubeMesh();
      const Ray ray{Float4{0.1f, 0.2f, -5.0f, 1.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = mesh->findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(4.5f, distance, 0.001f, L"hit -z face at z=-0.5", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionCubeMiss) {
      auto mesh = makeCubeMesh();
      const Ray ray{Float4{5.0f, 5.0f, -5.0f, 1.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = mesh->findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(100.0f, distance, L"ray outside cube should miss", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionTriangleSetsNormal) {
      // Triangle vertices in winding order (-0.5,-0.5,0), (0,0.5,0), (0.5,-0.5,0). cross3(edge1,
      // edge2) gives (0,0,-1), so the flat normal points in -z and faces toward the camera at -5.
      auto mesh = makeTriangleMesh();
      const Ray ray{Float4{0.0f, -0.25f, -5.0f, 1.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      mesh->findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(0.0f, x(intersection.surfaceNormal), 0.001f, L"normal x", LINE_INFO());
      Assert::AreEqual(0.0f, y(intersection.surfaceNormal), 0.001f, L"normal y", LINE_INFO());
      Assert::AreEqual(-1.0f, z(intersection.surfaceNormal), 0.001f, L"normal z (flat normal is -z)", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionTriangleSetsVertex) {
      auto mesh = makeTriangleMesh();
      const Ray ray{Float4{0.0f, -0.25f, -5.0f, 1.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      mesh->findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(0.0f, x(intersection.vertex), 0.001f, L"vertex x", LINE_INFO());
      Assert::AreEqual(-0.25f, y(intersection.vertex), 0.001f, L"vertex y", LINE_INFO());
      Assert::AreEqual(0.0f, z(intersection.vertex), 0.001f, L"vertex z", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionTriangleSetsTexCoords) {
      auto mesh = makeTriangleMesh();
      const Ray ray{Float4{0.0f, -0.25f, -5.0f, 1.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      mesh->findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(1.0f, w(intersection.texCoords), 0.001f, L"texCoord w", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionBackfaceCulledMisses) {
      // Triangle's flat normal is (0,0,-1). Approaching from +z toward -z is a backface hit
      // (determinant > 0). With cullingMask.x=true the brute-force loop's culledBack check
      // skips the facet.
      auto mesh = makeTriangleMesh();
      const Ray ray{Float4{0.0f, -0.25f, 5.0f, 1.0f}, Float4{0.0f, 0.0f, -1.0f, 0.0f}};
      const auto culling = blend<true, false, false, false>(Zero<Float4::VectorBoolType>(), MaskAll<Float4::VectorBoolType>());
      const RayCast rayCast{ray, culling, Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = mesh->findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(100.0f, distance, L"backface culled should miss", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionFrontfaceCulledMisses) {
      auto mesh = makeTriangleMesh();
      const Ray ray{Float4{0.0f, -0.25f, -5.0f, 1.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const auto culling = blend<false, true, false, false>(Zero<Float4::VectorBoolType>(), MaskAll<Float4::VectorBoolType>());
      const RayCast rayCast{ray, culling, Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = mesh->findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(100.0f, distance, L"frontface culled should miss frontface hit", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionBeyondMaxDistance) {
      auto mesh = makeTriangleMesh();
      const Ray ray{Float4{0.0f, -0.25f, -5.0f, 1.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 3.0f};
      FacetIntersection intersection{};

      const auto distance = mesh->findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(3.0f, distance, L"hit beyond maxDistance returns maxDistance", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionJustWithinMaxDistance) {
      auto mesh = makeTriangleMesh();
      const Ray ray{Float4{0.0f, -0.25f, -5.0f, 1.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 5.001f};
      FacetIntersection intersection{};

      const auto distance = mesh->findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(5.0f, distance, 0.001f, L"hit just within maxDistance", LINE_INFO());
    }

    TEST_METHOD(findAnyIntersectionTriangleHit) {
      auto mesh = makeTriangleMesh();
      const Ray ray{Float4{0.0f, -0.25f, -5.0f, 1.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = mesh->findAnyIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(5.0f, distance, 0.001f, L"findAny should hit triangle", LINE_INFO());
    }

    TEST_METHOD(findAnyIntersectionTriangleMiss) {
      auto mesh = makeTriangleMesh();
      const Ray ray{Float4{0.0f, 5.0f, -5.0f, 1.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = mesh->findAnyIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(100.0f, distance, L"findAny miss should return maxDistance", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionSelfOcclusion) {
      auto mesh = makeTriangleMesh();
      const Ray ray{Float4{0.0f, -0.25f, -5.0f, 1.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection priorHit{};

      mesh->findNearestIntersection(rayCast, nullptr, priorHit);
      Assert::IsNotNull(priorHit.node, L"first cast should record an intersected node", LINE_INFO());

      FacetIntersection followUp{};
      const auto distance = mesh->findNearestIntersection(rayCast, &priorHit, followUp);

      Assert::AreEqual(100.0f, distance, L"self-occlusion should skip the only facet", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionSmoothedNormalWithSingleFacet) {
      auto mesh = makeTriangleMesh();
      const Ray ray{Float4{0.0f, -0.25f, -5.0f, 1.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      mesh->findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(z(intersection.surfaceNormal), z(intersection.smoothedNormal), 0.001f,
                       L"smoothed normal z should match surface normal", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionModelSpaceMatchesWorldSpace) {
      auto mesh = makeTriangleMesh();
      const Ray ray{Float4{0.0f, -0.25f, -5.0f, 1.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      mesh->findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(z(intersection.vertex), z(intersection.msVertex), 0.001f,
                       L"msVertex should match vertex in untransformed mesh", LINE_INFO());
      Assert::AreEqual(z(intersection.surfaceNormal), z(intersection.msSurfaceNormal), 0.001f,
                       L"msSurfaceNormal should match surfaceNormal", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionRayParallelToTriangle) {
      // Ray parallel to triangle plane (z=0) at z = -0.25; should miss.
      auto mesh = makeTriangleMesh();
      const Ray ray{Float4{-5.0f, -0.25f, -0.25f, 1.0f}, Float4{1.0f, 0.0f, 0.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = mesh->findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(100.0f, distance, L"ray parallel to triangle plane should miss", LINE_INFO());
    }

    TEST_METHOD(findNearestIntersectionCubeFromInsideHitsBackWall) {
      // Origin near center, going +z; first wall hit is at z=+0.5. Offset xy from the cube-face
      // triangulation diagonal which Havel rejects.
      auto mesh = makeCubeMesh();
      const Ray ray{Float4{0.1f, 0.2f, 0.0f, 1.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = mesh->findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(0.5f, distance, 0.001f, L"from origin should hit +z wall at distance 0.5", LINE_INFO());
    }

    TEST_METHOD(buildPlaneMeshHasTwoFacets) {
      // Mesh::buildPlaneMesh is also a public helper; verify shape.
      auto mesh = Mesh::buildPlaneMesh();
      Assert::AreEqual(ASizeT{2}, mesh->getFaceCount(), L"plane mesh face count", LINE_INFO());
    }

    TEST_METHOD(planeMeshIntersectionWorks) {
      // Plane mesh: 4 vertices forming z=0 quad. After computeStandardMesh: scaled to [-0.5, 0.5]
      // in xy at z=0. Offset xy to avoid the diagonal between the two triangles.
      auto mesh = Mesh::buildPlaneMesh();
      const Ray ray{Float4{0.1f, 0.2f, -5.0f, 1.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};

      const auto distance = mesh->findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(5.0f, distance, 0.001f, L"plane mesh hit at z=0", LINE_INFO());
    }

    TEST_METHOD(leafBlocksMatchNodeLoop) {
      // A span inside the KD-tree's leafGeometry takes the packed block path, a copy of the same node pointers the
      // single-facet loop. Every sub-range of the cube's single leaf (12 facets: ranges start and end mid-block),
      // each culling mode and each facet of the range as the origin must give the same facet; the distances may
      // differ in the last bits, as the block path's dot products are FMA chains and dotv / dot3v aren't.
      auto mesh = makeCubeMesh();
      mesh->setTreeBalancer(std::make_unique<MedianKDTreeBalancer>());
      const auto& leafGeometry = mesh->getGraph()->leafGeometry;
      Assert::AreEqual(ASizeT{12}, leafGeometry.size(), L"cube is a single leaf", LINE_INFO());
      const auto copiedNodes = PGeometryNodeList(leafGeometry);

      auto generator = std::mt19937{0x5eed1234u};
      auto uniform = std::uniform_real_distribution<Float>(-1.0f, 1.0f);
      std::vector<Ray> rays;
      for (auto i = 0; i < 32; ++i) {
        // from outside towards a point in the cube, and from inside the cube in any direction
        const auto origin = Float4{3.0f * uniform(generator), 3.0f * uniform(generator), -3.0f, 1.0f};
        const auto target =
          Float4{0.5f * uniform(generator), 0.5f * uniform(generator), 0.5f * uniform(generator), 1.0f};
        rays.emplace_back(origin, normalize3(target - origin));
        const auto inside =
          Float4{0.4f * uniform(generator), 0.4f * uniform(generator), 0.4f * uniform(generator), 1.0f};
        const auto direction = Float4{uniform(generator), uniform(generator), uniform(generator), 0.0f};
        rays.emplace_back(inside, normalize3(direction));
      }

      // relative to distances of up to about 6
      constexpr auto DistanceTolerance = 1e-5f;
      auto hits = 0;
      for (const auto culling : {-1, 0, 1}) {
        for (const auto& ray : rays) {
          const RayCast rayCast{ray, cullingOrientationToMask(culling), Size2(0, 0), 100.0f};
          for (auto begin = ASizeT{0}; begin < leafGeometry.size(); ++begin) {
            for (auto end = begin + 1; end <= leafGeometry.size(); ++end) {
              const auto blocks = GeometryNodeSpan(leafGeometry.data() + begin, end - begin);
              const auto nodes = GeometryNodeSpan(copiedNodes.data() + begin, end - begin);
              for (auto origin = begin; origin <= end; ++origin) {
                // origin == end: no origin intersection
                auto originIntersection = FacetIntersection{};
                originIntersection.node = origin < end ? leafGeometry[origin] : nullptr;
                const auto* originPointer = origin < end ? &originIntersection : nullptr;

                auto blockHit = FacetIntersection{};
                auto nodeHit = FacetIntersection{};
                const auto blockDistance = mesh->findNearestIntersection(blocks, rayCast, originPointer, blockHit);
                const auto nodeDistance = mesh->findNearestIntersection(nodes, rayCast, originPointer, nodeHit);
                Assert::AreEqual(nodeDistance, blockDistance, DistanceTolerance, L"nearest distance", LINE_INFO());
                Assert::AreEqual(
                    nodeDistance < rayCast.maxDistance, blockDistance < rayCast.maxDistance, L"nearest hit", LINE_INFO()
                );
                if (nodeDistance < rayCast.maxDistance) {
                  Assert::IsTrue(nodeHit.node == blockHit.node, L"nearest facet", LINE_INFO());
                  ++hits;
                }

                auto blockAny = FacetIntersection{};
                auto nodeAny = FacetIntersection{};
                const auto blockAnyDistance = mesh->findAnyIntersection(blocks, rayCast, originPointer, blockAny);
                const auto nodeAnyDistance = mesh->findAnyIntersection(nodes, rayCast, originPointer, nodeAny);
                Assert::AreEqual(
                    nodeAnyDistance, blockAnyDistance, DistanceTolerance, L"any-hit distance", LINE_INFO()
                );
                Assert::AreEqual(
                    nodeAnyDistance < rayCast.maxDistance, blockAnyDistance < rayCast.maxDistance, L"any hit",
                    LINE_INFO()
                );
                if (nodeAnyDistance < rayCast.maxDistance) {
                  Assert::IsTrue(nodeAny.node == blockAny.node, L"any-hit facet", LINE_INFO());
                }
              }
            }
          }
        }
      }
      Assert::IsTrue(hits > 1000, L"enough hits to compare", LINE_INFO());
    }

    TEST_METHOD(builtMeshWithTraverserAndBalancerHasKDTree) {
      const auto mesh = Mesh::buildCubeMesh(
          std::make_unique<WideKDTreeTraverser<FacetIntersection>>(), std::make_unique<BruteForceSAHKDTreeBalancer>()
      );
      Assert::IsNotNull(mesh->getGraph(), L"KD-tree built", LINE_INFO());
      Assert::IsNull(makeCubeMesh()->getGraph(), L"no KD-tree without a balancer", LINE_INFO());
    }

    TEST_METHOD(builtMeshIntersectsThroughItsTraverser) {
      auto traverser = std::make_unique<CountingKDTreeTraverser>();
      const auto &counter = *traverser;
      const auto mesh = Mesh::buildCubeMesh(std::move(traverser), std::make_unique<BruteForceSAHKDTreeBalancer>());

      const Ray ray{Float4{0.1f, 0.2f, -5.0f, 1.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};
      Assert::AreEqual(
          4.5f, mesh->findNearestIntersection(rayCast, nullptr, intersection), 0.001f, L"nearest", LINE_INFO()
      );
      // without culling, any hit may also be the back face
      Assert::IsTrue(mesh->findAnyIntersection(rayCast, nullptr, intersection) < rayCast.maxDistance, L"any",
                     LINE_INFO());
      Assert::AreEqual(2, counter.queries, L"both queries traverse the KD-tree", LINE_INFO());
    }

    TEST_METHOD(kdTreeCubeMatchesBruteForce) {
      const auto bruteForce = makeCubeMesh();
      const auto naiveSAH = Mesh::buildCubeMesh(
          std::make_unique<NaiveKDTreeTraverser<FacetIntersection>>(), std::make_unique<BruteForceSAHKDTreeBalancer>()
      );
      const auto wideSAH = Mesh::buildCubeMesh(
          std::make_unique<WideKDTreeTraverser<FacetIntersection>>(), std::make_unique<BruteForceSAHKDTreeBalancer>()
      );
      const auto wideMedian = Mesh::buildCubeMesh(
          std::make_unique<WideKDTreeTraverser<FacetIntersection>>(), std::make_unique<MedianKDTreeBalancer>()
      );
      for (const auto *mesh : {naiveSAH.get(), wideSAH.get(), wideMedian.get()}) {
        Assert::IsTrue(assertSameIntersections(*bruteForce, *mesh) > 100, L"enough hits to compare", LINE_INFO());
      }
    }

    TEST_METHOD(kdTreePlaneAndTriangleMatchBruteForce) {
      // Rays towards the z = 0 plane from both sides, so the culling modes give hits and misses
      const auto planeBruteForce = Mesh::buildPlaneMesh();
      const auto plane = Mesh::buildPlaneMesh(
          std::make_unique<WideKDTreeTraverser<FacetIntersection>>(), std::make_unique<BruteForceSAHKDTreeBalancer>()
      );
      const auto triangleBruteForce = Mesh::buildTriangleMesh();
      const auto triangle = Mesh::buildTriangleMesh(
          std::make_unique<WideKDTreeTraverser<FacetIntersection>>(), std::make_unique<BruteForceSAHKDTreeBalancer>()
      );
      Assert::IsNotNull(plane->getGraph(), L"plane KD-tree", LINE_INFO());
      Assert::IsNotNull(triangle->getGraph(), L"triangle KD-tree", LINE_INFO());
      const auto pairs = {
          std::pair{planeBruteForce.get(), plane.get()}, std::pair{triangleBruteForce.get(), triangle.get()}
      };
      auto hits = 0;
      for (const auto culling : {-1, 0, 1}) {
        for (const auto side : {-1.0f, 1.0f}) {
          for (auto i = 0; i < 9; ++i) {
            const auto px = 0.3f * static_cast<Float>(i % 3 - 1) + 0.05f;
            const auto py = 0.3f * static_cast<Float>(i / 3 - 1) + 0.05f;
            const Ray ray{Float4{px, py, 5.0f * side, 1.0f}, Float4{0.0f, 0.0f, -side, 0.0f}};
            const RayCast rayCast{ray, cullingOrientationToMask(culling), Size2(0, 0), 100.0f};
            for (const auto &[expected, actual] : pairs) {
              auto expectedHit = FacetIntersection{};
              auto actualHit = FacetIntersection{};
              const auto distance = expected->findNearestIntersection(rayCast, nullptr, expectedHit);
              Assert::AreEqual(
                  distance, actual->findNearestIntersection(rayCast, nullptr, actualHit), 1e-5f, L"nearest distance",
                  LINE_INFO()
              );
              Assert::AreEqual(
                  expected->findAnyIntersection(rayCast, nullptr, expectedHit),
                  actual->findAnyIntersection(rayCast, nullptr, actualHit), 1e-5f, L"any-hit distance", LINE_INFO()
              );
              hits += distance < rayCast.maxDistance ? 1 : 0;
            }
          }
        }
      }
      Assert::IsTrue(hits > 10, L"enough hits to compare", LINE_INFO());
    }

    TEST_METHOD(setTreeBalancerWithoutTraverserKeepsTestingEveryFacet) {
      // Scene::setMeshTreeBalancer also reaches meshes built without a traverser
      const auto bruteForce = makeCubeMesh();
      const auto balanced = makeCubeMesh();
      balanced->setTreeBalancer(std::make_unique<BruteForceSAHKDTreeBalancer>());
      Assert::IsNotNull(balanced->getGraph(), L"KD-tree built", LINE_INFO());
      Assert::IsTrue(assertSameIntersections(*bruteForce, *balanced) > 100, L"enough hits to compare", LINE_INFO());
    }

    TEST_METHOD(loadingRemovesFacetsWithoutArea) {
      // A square of two facets, plus a facet with collinear vertices and one with a repeated vertex. Both share vertices
      // with the square: their zero normals must not reach its smoothed normals.
      TempFile file{".off"};
      {
        std::ofstream off{file.path};
        off << "OFF\n5 4 0\n-1 -1 0\n1 -1 0\n1 1 0\n-1 1 0\n0 -1 0\n";
        off << "3 0 1 2\n3 0 2 3\n3 0 4 1\n3 2 2 3\n";
      }
      const auto mesh = Mesh::loadFromOffFile(file.str(), false, nullptr, nullptr);
      Assert::AreEqual(ASizeT{2}, mesh->getFaceCount(), L"facets without area removed", LINE_INFO());

      const Ray ray{Float4{0.2f, -0.3f, -5.0f, 1.0f}, Float4{0.0f, 0.0f, 1.0f, 0.0f}};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      FacetIntersection intersection{};
      const auto distance = mesh->findNearestIntersection(rayCast, nullptr, intersection);

      Assert::AreEqual(5.0f, distance, 0.001f, L"hits the square", LINE_INFO());
      Assert::AreEqual(0.0f, x(intersection.smoothedNormal), 0.001f, L"smoothed normal x", LINE_INFO());
      Assert::AreEqual(0.0f, y(intersection.smoothedNormal), 0.001f, L"smoothed normal y", LINE_INFO());
      Assert::AreEqual(1.0f, z(intersection.smoothedNormal), 0.001f, L"smoothed normal z", LINE_INFO());
    }
  };
}
