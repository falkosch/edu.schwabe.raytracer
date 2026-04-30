#include "raytracing_tests.h"

#include "raytracing/geometry/forms/meshes/Mesh.h"

namespace raytracer::test {
  TEST_CLASS(MeshTest) {
    // Mesh::buildXxxMesh static helpers default-construct (no traverser, no balancer), so the
    // returned mesh has no KD-tree and findNearestIntersection takes the brute-force fallback over
    // the geometryNodes list. Note: setupMesh() rescales/translates the input vertices via
    // computeStandardMesh() — the input shape is centered at the origin and uniformly scaled to fit
    // within [-0.5, 0.5] along the axes with non-zero extent.
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
  };
}
