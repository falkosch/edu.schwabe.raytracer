#include "raytracing/geometry/forms/meshes/Mesh.h"
#include "../../../../stdafx.h"

#include "raytracing/geometry/forms/meshes/MeshGeometryNode.h"
#include "raytracing/geometry/forms/meshes/facets.h"
#include "raytracing/geometry/forms/meshes/textureCoordinates.h"
#include "raytracing/geometry/forms/meshes/vertices.h"

#include <algorithm>
#include <array>
#include <bit>
#include <cassert>
#include <fstream>
#include <functional>
#include <ios>
#include <limits>
#include <logging.h>
#include <sstream>
#include <string>

static const auto Log = logging::scope("Mesh");

namespace raytracer
{
  Mesh::Mesh()
    : bounding(), vertices(), vertexNormals(), facetIndices(), facets(), texCoords(), flatNormals(), smoothNormals(),
      planeNormals(), facetEdges(), leafPlaneNormalBlocks(), leafFacetIndices(), nodes(), graph(), traverser(),
      balancer()
  {
  }

  Mesh::Mesh(
    std::unique_ptr<const KDTreeTraverser<FacetIntersection>> traverser,
    std::unique_ptr<const KDTreeBalancer> balancer
  )
    : bounding(), vertices(), vertexNormals(), facetIndices(), facets(), texCoords(), flatNormals(), smoothNormals(),
      planeNormals(), facetEdges(), leafPlaneNormalBlocks(), leafFacetIndices(), nodes(), graph(),
      traverser(std::move(traverser)), balancer(std::move(balancer))
  {
  }

  Mesh::~Mesh()
  {
    clear();
  }

  AxisAlignedBoundingBox Mesh::getBounding() const
  {
    return bounding;
  }

  ASizeT Mesh::getFaceCount() const
  {
    return static_cast<ASizeT>(facets.size());
  }

  Float Mesh::findNearestIntersection(
    const RayCast& rayCast, const FacetIntersection* const originIntersection, FacetIntersection& intersectionOut
  ) const
  {
    if (graph)
    {
      return traverser->findNearestIntersection(*this, *graph, rayCast, originIntersection, intersectionOut);
    }
    return findNearestIntersection(nodes, rayCast, originIntersection, intersectionOut);
  }

  Float Mesh::findAnyIntersection(
    const RayCast& rayCast, const FacetIntersection* const originIntersection, FacetIntersection& intersectionOut
  ) const
  {
    if (graph)
    {
      return traverser->findAnyIntersection(*this, *graph, rayCast, originIntersection, intersectionOut);
    }
    return findAnyIntersection(nodes, rayCast, originIntersection, intersectionOut);
  }

  namespace
  {
    constexpr auto NoLeafOffset = std::numeric_limits<ASizeT>::max();
    constexpr auto NoEntry = std::numeric_limits<ASizeT>::max();
    constexpr ASizeT BlockWidth{4};

    // Offset of geometryNodes in graph->leafGeometry, or NoLeafOffset if the span lies elsewhere (e.g. the node list of
    // a mesh without KD-tree).
    ASizeT leafOffsetOf(const KDTreeRoot* const graph, const GeometryNodeSpan geometryNodes) noexcept
    {
      if (!graph)
      {
        return NoLeafOffset;
      }
      const auto* const begin = graph->leafGeometry.data();
      const auto* const end = begin + graph->leafGeometry.size();
      const auto less = std::less<GeometryNode* const*>();
      if (less(geometryNodes.data(), begin) || less(end, geometryNodes.data() + geometryNodes.size()))
      {
        return NoLeafOffset;
      }
      return static_cast<ASizeT>(geometryNodes.data() - begin);
    }

    // Entry of the origin's facet in the leaf starting at offset, or NoEntry.
    ASizeT originEntryOf(
      const GeometryNodeSpan geometryNodes, const ASizeT offset, const FacetIntersection* const originIntersection
    ) noexcept
    {
      if (originIntersection)
      {
        for (auto i = ASizeT{0}; i < geometryNodes.size(); ++i)
        {
          if (originIntersection->node == geometryNodes[i])
          {
            return offset + i;
          }
        }
      }
      return NoEntry;
    }

    // planeNormals.v0 is the facet's unnormalized flat normal (flatNormals = normalize3(planeNormals.v0)), so its
    // determinant has the flat normal's sign
    bool culled(const FacetNormals& planeNormals, const RayCast& rayCast) noexcept
    {
      const auto frontfaced = isNegative(dot3v(planeNormals.v0, rayCast.ray.direction));
      const auto culledBack = (!frontfaced) & backfaceCulled(rayCast);
      const auto culledFront = (!!frontfaced) & frontfaceCulled(rayCast);
      return static_cast<bool>(culledBack | culledFront);
    }

    // The ray's components, each broadcast to all lanes.
    struct RayLanes
    {
      std::array<Float4, 4> origin, direction;

      explicit RayLanes(const Ray& ray) noexcept
        : origin{xxxx(ray.origin), yyyy(ray.origin), zzzz(ray.origin), wwww(ray.origin)},
          direction{xxxx(ray.direction), yyyy(ray.direction), zzzz(ray.direction), wwww(ray.direction)}
      {
      }
    };

    // Lane-wise dot products, summed like _mm_dp_ps in dotv / dot3v: (p0 + p1) + (p2 + p3), with p3 = +0 for the
    // 3-component product. So every lane gets exactly the single-facet test's values.
    Float4 dotLanes(const std::array<Float4, 4>& a, const std::array<Float4, 4>& b) noexcept
    {
      return (a[0] * b[0] + a[1] * b[1]) + (a[2] * b[2] + a[3] * b[3]);
    }

    Float4 dot3Lanes(const std::array<Float4, 4>& a, const std::array<Float4, 4>& b) noexcept
    {
      return (a[0] * b[0] + a[1] * b[1]) + (a[2] * b[2] + Zero<Float4>());
    }

    struct BlockTest
    {
      Float4 determinant, detT, detU, detV;
      // bit i: lane i is a candidate, isn't culled and passes the three sign tests of the Havel test
      unsigned hits;
    };

    // nearestIntersectionHavel for the 4 facets of a block at once: the same operations in the same order, lane-wise
    BlockTest testBlock(
      const FacetNormalsBlock& block, const RayLanes& ray, const Float4& maxDistance, const unsigned candidates,
      const bool cullBack, const bool cullFront
    ) noexcept
    {
      const auto determinant = dot3Lanes(block.v0, ray.direction);
      const auto frontfaced = static_cast<unsigned>(isNegative(determinant));
      const auto culledLanes = (cullBack ? ~frontfaced & 0xFu : 0u) | (cullFront ? frontfaced : 0u);

      const auto detT = -dotLanes(block.v0, ray.origin);
      const auto checkT = multiplySub(determinant, maxDistance, detT);

      // intersection point scaled by the determinant
      auto point = std::array<Float4, 4>();
      for (auto c = ASizeT{0}; c < point.size(); ++c)
      {
        point[c] = multiplyAdd(ray.origin[c], determinant, ray.direction[c] * detT);
      }
      const auto detU = dotLanes(point, block.v1);
      const auto checkU = determinant - detU;
      const auto detV = dotLanes(point, block.v2);
      const auto checkV = determinant - (detU + detV);

      const auto rejected = static_cast<unsigned>(
        isNegative(detT ^ checkT) | isNegative(detU ^ checkU) | isNegative(detV ^ checkV)
      );
      return BlockTest{determinant, detT, detU, detV, candidates & ~culledLanes & ~rejected};
    }

    // bit i set where entry first + i lies in [begin, end) and isn't the origin's entry
    unsigned candidateLanes(const ASizeT first, const ASizeT begin, const ASizeT end, const ASizeT originEntry) noexcept
    {
      const auto low = begin > first ? begin - first : ASizeT{0};
      const auto high = std::min(end - first, BlockWidth);
      auto lanes = ((1u << high) - 1u) & ~((1u << low) - 1u);
      if (originEntry - first < BlockWidth)
      {
        lanes &= ~(1u << (originEntry - first));
      }
      return lanes;
    }

    // { u, v, distance, distance } of a lane, as nearestIntersectionHavel returns it
    Float4 laneTexCoordsAndDistance(const BlockTest& test, const int lane) noexcept
    {
      return Float4{test.detU[lane], test.detV[lane], test.detT[lane], test.detT[lane]} /
        Float4{test.determinant[lane]};
    }

    // Nearest hit among the entries [begin, end): returns { u, v, distance } and sets nearestEntry, or returns
    // maxDistance. Hits are taken in entry order against the nearest hit so far, as in the single-facet loop.
    Float4 nearestHitInBlocks(
      const std::vector<FacetNormalsBlock>& blocks, const ASizeT begin, const ASizeT end, const ASizeT originEntry,
      const RayCast& rayCast, ASizeT& nearestEntry
    ) noexcept
    {
      const auto ray = RayLanes(rayCast.ray);
      const auto cullBack = backfaceCulled(rayCast);
      const auto cullFront = frontfaceCulled(rayCast);

      auto nearestTexCoordsAndDistance = Float4{rayCast.maxDistance};
      for (auto first = begin / BlockWidth * BlockWidth; first < end; first += BlockWidth)
      {
        const auto test = testBlock(
          blocks[first / BlockWidth], ray, zzzz(nearestTexCoordsAndDistance),
          candidateLanes(first, begin, end, originEntry), cullBack, cullFront
        );
        for (auto hits = test.hits; hits != 0; hits &= hits - 1)
        {
          const auto lane = std::countr_zero(hits);
          // a hit in an earlier lane may have moved the nearest distance below this lane's: test it again
          const auto determinant = Float4{test.determinant[lane]};
          const auto detT = Float4{test.detT[lane]};
          if (isNegative(detT ^ multiplySub(determinant, zzzz(nearestTexCoordsAndDistance), detT)))
          {
            continue;
          }
          const auto facetTexCoordsAndDistance = laneTexCoordsAndDistance(test, lane);
          if (z(facetTexCoordsAndDistance < nearestTexCoordsAndDistance))
          {
            nearestTexCoordsAndDistance = facetTexCoordsAndDistance;
            nearestEntry = first + static_cast<ASizeT>(lane);
          }
        }
      }
      return nearestTexCoordsAndDistance;
    }

    // Like nearestHitInBlocks, but returns the first hit in entry order.
    Float4 anyHitInBlocks(
      const std::vector<FacetNormalsBlock>& blocks, const ASizeT begin, const ASizeT end, const ASizeT originEntry,
      const RayCast& rayCast, ASizeT& hitEntry
    ) noexcept
    {
      const auto ray = RayLanes(rayCast.ray);
      const auto cullBack = backfaceCulled(rayCast);
      const auto cullFront = frontfaceCulled(rayCast);

      const auto maxDistance = Float4(rayCast.maxDistance);
      for (auto first = begin / BlockWidth * BlockWidth; first < end; first += BlockWidth)
      {
        const auto test = testBlock(
          blocks[first / BlockWidth], ray, maxDistance, candidateLanes(first, begin, end, originEntry), cullBack,
          cullFront
        );
        for (auto hits = test.hits; hits != 0; hits &= hits - 1)
        {
          const auto lane = std::countr_zero(hits);
          const auto facetTexCoordsAndDistance = laneTexCoordsAndDistance(test, lane);

          // check whether the distance in t is nearer than the distance in preserved d
          if (z(facetTexCoordsAndDistance < maxDistance))
          {
            hitEntry = first + static_cast<ASizeT>(lane);
            return facetTexCoordsAndDistance;
          }
        }
      }
      return maxDistance;
    }

    // The single-facet loop over a node list that isn't a leaf of the KD-tree.
    Float4 nearestHitInNodes(
      const GeometryNodeSpan geometryNodes, const RayCast& rayCast, const FacetIntersection* const originIntersection,
      const std::vector<FacetNormals>& planeNormals, ASizeT& nearestIndex
    ) noexcept
    {
      auto nearestTexCoordsAndDistance = Float4{rayCast.maxDistance};
      for (const auto node : geometryNodes)
      {
        // avoid self-occlusion
        if (originIntersection && originIntersection->node == node)
        {
          continue;
        }

        const auto index = static_cast<const MeshGeometryNode*>(node)->index;
        if (culled(planeNormals[index], rayCast))
        {
          continue;
        }

        // auto facetTexCoordsAndDistance =
        //     nearestIntersectionMoeller(facets[index].v0, facetEdges[index], rayCast.ray,
        //     nearestTexCoordsAndDistance);
        const auto facetTexCoordsAndDistance =
          nearestIntersectionHavel(planeNormals[index], rayCast, zzzz(nearestTexCoordsAndDistance));
        if (z(facetTexCoordsAndDistance < nearestTexCoordsAndDistance))
        {
          nearestTexCoordsAndDistance = facetTexCoordsAndDistance;
          nearestIndex = index;
        }
      }
      return nearestTexCoordsAndDistance;
    }

    // Like nearestHitInNodes, but stops at the first hit.
    Float4 anyHitInNodes(
      const GeometryNodeSpan geometryNodes, const RayCast& rayCast, const FacetIntersection* const originIntersection,
      const std::vector<FacetNormals>& planeNormals, ASizeT& hitIndex
    ) noexcept
    {
      const auto maxDistance = Float4(rayCast.maxDistance);
      for (const auto node : geometryNodes)
      {
        // avoid self-occlusion
        if (originIntersection && originIntersection->node == node)
        {
          continue;
        }

        const auto index = static_cast<const MeshGeometryNode*>(node)->index;
        if (culled(planeNormals[index], rayCast))
        {
          continue;
        }

        // auto facetTexCoordsAndDistance =
        //     nearestIntersectionMoeller(facets[index].v0, facetEdges[index], rayCast.ray, maxDistance);
        const auto facetTexCoordsAndDistance =
          nearestIntersectionHavel(planeNormals[index], rayCast, zzzz(maxDistance));

        // check whether the distance in t is nearer than the distance in preserved d
        if (z(facetTexCoordsAndDistance < maxDistance))
        {
          hitIndex = index;
          return facetTexCoordsAndDistance;
        }
      }
      return maxDistance;
    }
  }

  Float Mesh::findNearestIntersection(
    const GeometryNodeSpan geometryNodes, const RayCast& rayCast, const FacetIntersection* const originIntersection,
    FacetIntersection& intersectionOut
  ) const
  {
    if (outOfReach(rayCast, Zero<Float>()))
    {
      return rayCast.maxDistance;
    }

    ASizeT nearestIndex{0};
    Float4 nearestTexCoordsAndDistance;
    if (const auto offset = leafOffsetOf(graph.get(), geometryNodes); offset != NoLeafOffset)
    {
      auto nearestEntry = NoEntry;
      nearestTexCoordsAndDistance = nearestHitInBlocks(
        leafPlaneNormalBlocks, offset, offset + geometryNodes.size(),
        originEntryOf(geometryNodes, offset, originIntersection), rayCast, nearestEntry
      );
      if (nearestEntry != NoEntry)
      {
        nearestIndex = leafFacetIndices[nearestEntry];
      }
    }
    else
    {
      nearestTexCoordsAndDistance =
        nearestHitInNodes(geometryNodes, rayCast, originIntersection, planeNormals, nearestIndex);
    }

    return computeFacetIntersection(
      nearestIndex, nearestTexCoordsAndDistance, rayCast, texCoords, flatNormals, smoothNormals, nodes,
      intersectionOut
    );
  }

  // Finds any intersection of a Ray within a geometry.
  Float Mesh::findAnyIntersection(
    const GeometryNodeSpan geometryNodes, const RayCast& rayCast, const FacetIntersection* const originIntersection,
    FacetIntersection& intersectionOut
  ) const
  {
    if (outOfReach(rayCast, Zero<Float>()))
    {
      return rayCast.maxDistance;
    }

    ASizeT hitIndex{0};
    Float4 hitTexCoordsAndDistance;
    if (const auto offset = leafOffsetOf(graph.get(), geometryNodes); offset != NoLeafOffset)
    {
      auto hitEntry = NoEntry;
      hitTexCoordsAndDistance = anyHitInBlocks(
        leafPlaneNormalBlocks, offset, offset + geometryNodes.size(),
        originEntryOf(geometryNodes, offset, originIntersection), rayCast, hitEntry
      );
      if (hitEntry != NoEntry)
      {
        hitIndex = leafFacetIndices[hitEntry];
      }
    }
    else
    {
      hitTexCoordsAndDistance = anyHitInNodes(geometryNodes, rayCast, originIntersection, planeNormals, hitIndex);
    }

    return computeFacetIntersection(
      hitIndex, hitTexCoordsAndDistance, rayCast, texCoords, flatNormals, smoothNormals, nodes, intersectionOut
    );
  }

  std::unique_ptr<Mesh> Mesh::buildCubeMesh()
  {
    auto mesh = std::make_unique<Mesh>();

    mesh->vertices.reserve(8);
    mesh->vertices.push_back(Float4(-1.0f, -1.0f, -1.0f, 1.0f));
    mesh->vertices.push_back(Float4(-1.0f, -1.0f, 1.0f, 1.0f));
    mesh->vertices.push_back(Float4(1.0f, -1.0f, 1.0f, 1.0f));
    mesh->vertices.push_back(Float4(1.0f, -1.0f, -1.0f, 1.0f));
    mesh->vertices.push_back(Float4(-1.0f, 1.0f, -1.0f, 1.0f));
    mesh->vertices.push_back(Float4(-1.0f, 1.0f, 1.0f, 1.0f));
    mesh->vertices.push_back(Float4(1.0f, 1.0f, 1.0f, 1.0f));
    mesh->vertices.push_back(Float4(1.0f, 1.0f, -1.0f, 1.0f));

    mesh->facetIndices.reserve(12);
    // 2 x bottom facets
    mesh->facetIndices.push_back(UInt3(0, 2, 1));
    mesh->facetIndices.push_back(UInt3(2, 0, 3));
    // 2 x top facets
    mesh->facetIndices.push_back(UInt3(4, 5, 6));
    mesh->facetIndices.push_back(UInt3(6, 7, 4));
    // 2 x left facets
    mesh->facetIndices.push_back(UInt3(0, 1, 5));
    mesh->facetIndices.push_back(UInt3(5, 4, 0));
    // 2 x front facets
    mesh->facetIndices.push_back(UInt3(1, 2, 6));
    mesh->facetIndices.push_back(UInt3(6, 5, 1));
    // 2 x right facets
    mesh->facetIndices.push_back(UInt3(2, 3, 7));
    mesh->facetIndices.push_back(UInt3(7, 6, 2));
    // 2 x back facets
    mesh->facetIndices.push_back(UInt3(3, 0, 4));
    mesh->facetIndices.push_back(UInt3(4, 7, 3));

    mesh->setupMesh();
    computeTexCoordsOrtho(
      mesh->facetIndices, mesh->facets, Float4{1.0f, 1.0f, 0.0f, 0.25f}, Float4{0.0f, 1.0f, 1.0f, 0.25f},
      mesh->texCoords
    );

    return mesh;
  }

  std::unique_ptr<Mesh> Mesh::buildPlaneMesh()
  {
    auto mesh = std::make_unique<Mesh>();

    mesh->vertices.reserve(4);
    mesh->vertices.push_back(Float4{-1.0f, -1.0f, 0.0f, 1.0f});
    mesh->vertices.push_back(Float4{-1.0f, 1.0f, 0.0f, 1.0f});
    mesh->vertices.push_back(Float4{1.0f, 1.0f, 0.0f, 1.0f});
    mesh->vertices.push_back(Float4{1.0f, -1.0f, 0.0f, 1.0f});

    mesh->facetIndices.reserve(2);
    mesh->facetIndices.push_back(UInt3{0, 1, 2});
    mesh->facetIndices.push_back(UInt3{2, 3, 0});

    mesh->setupMesh();
    computeTexCoordsOrtho(
      mesh->facetIndices, mesh->facets, Float4{1.0f, 0.0f, 0.0f, 0.5f}, Float4{0.0f, 1.0f, 0.0f, 0.5f},
      mesh->texCoords
    );

    return mesh;
  }

  std::unique_ptr<Mesh> Mesh::buildTriangleMesh()
  {
    auto mesh = std::make_unique<Mesh>();

    mesh->vertices.reserve(3);
    mesh->vertices.push_back(Float4{-0.5f, 0.0f, 0.0f, 1.0f});
    mesh->vertices.push_back(Float4{0.0f, 1.0f, 0.0f, 1.0f});
    mesh->vertices.push_back(Float4{0.5f, 0.0f, 0.0f, 1.0f});

    mesh->facetIndices.push_back(UInt3{0, 1, 2});

    mesh->setupMesh();
    computeTexCoordsOrtho(
      mesh->facetIndices, mesh->facets, Float4{1.0f, 0.0f, 0.0f, 0.5f}, Float4{0.0f, 1.0f, 0.0f, 0.5f},
      mesh->texCoords
    );

    return mesh;
  }

  bool readMeshFileContent(std::ifstream file, std::string& stringOut)
  {
    if (file.is_open())
    {
      stringOut = std::string{std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
      file.close();
      return true;
    }
    return false;
  }

  void readNextNonEmptyLineInOFF(std::istringstream& fileStream, std::string& line)
  {
    line = "";
    while (line.empty())
    {
      std::getline(fileStream, line);
    }
  }

  bool readAndCheckHeaderInOFF(std::istringstream& fileStream, std::string& line)
  {
    while (std::getline(fileStream, line))
    {
      if (line == "OFF" || line == "OFF\r")
      {
        return true;
      }
    }
    return false;
  }

  std::unique_ptr<Mesh> Mesh::loadFromOffFile(
    const std::string& filename, const bool flipNormals,
    std::unique_ptr<const KDTreeTraverser<FacetIntersection>> traverser,
    std::unique_ptr<const KDTreeBalancer> balancer
  )
  {
    auto mesh = std::make_unique<Mesh>(std::move(traverser), std::move(balancer));
    if (filename.empty())
    {
      Log.error([] { return "File name is empty"; });
      return mesh;
    }

    std::string fileContent;
    if (!readMeshFileContent(std::ifstream{filename}, fileContent))
    {
      Log.error([fn = filename] { return "Failed to open " + fn; });
      return mesh;
    }
    Log.info([sz = fileContent.size(), fn = filename]
    {
      return "Loaded " + std::to_string(sz) + " bytes from file " + fn;
    });

    std::istringstream fileStream{fileContent};
    std::string line;
    if (!readAndCheckHeaderInOFF(fileStream, line))
    {
      Log.error([fn = filename] { return "File " + fn + " is not an OFF-file"; });
      return mesh;
    }

    readNextNonEmptyLineInOFF(fileStream, line);

    // read the number of vertices and faces
    ASizeT verticesCount, facetsCount;
    std::istringstream{line} >> verticesCount >> facetsCount;
    assert(verticesCount > 0);
    assert(facetsCount > 0);

    // read vertices
    mesh->vertices.resize(verticesCount);
    for (ASizeT i{0}; i < verticesCount; i++)
    {
      readNextNonEmptyLineInOFF(fileStream, line);

      Float x, y, z;
      std::istringstream{line} >> x >> y >> z;
      mesh->vertices[i] = Float4{x, y, z, One<Float>()};
    }

    // read faces
    mesh->facetIndices.resize(facetsCount);
    for (ASizeT i{0}; i < facetsCount; i++)
    {
      readNextNonEmptyLineInOFF(fileStream, line);

      std::istringstream lineStream{line};

      ASizeT faceVerticesCount;
      lineStream >> faceVerticesCount;
      if (faceVerticesCount != 3)
      {
        Log.error([fn = filename] { return fn + " is corrupted!"; });
        return mesh;
      }

      UInt v1, v2, v3;
      lineStream >> v1 >> v2 >> v3;
      if (flipNormals)
      {
        mesh->facetIndices[i] = UInt3{v1, v3, v2};
      }
      else
      {
        mesh->facetIndices[i] = UInt3{v1, v2, v3};
      }
    }

    mesh->setupMesh();
    computeTexCoordsSpherical(mesh->facetIndices, mesh->facets, mesh->texCoords);

    Log.info([fn = filename, vc = verticesCount, fc = facetsCount]
    {
      return "loaded " + fn + ": " + std::to_string(vc) + " vertices, " + std::to_string(fc) + " faces";
    });

    return mesh;
  }

  void Mesh::setTreeBalancer(std::unique_ptr<const KDTreeBalancer> newBalancer)
  {
    this->balancer = std::move(newBalancer);
    graph.reset();
    for (const auto* node : nodes)
    {
      delete node;
    }
    nodes.clear();
    Log.info([n = facets.size()] { return "Building culling tree for mesh (" + std::to_string(n) + " faces) ..."; });
    graph = computeNodesAndGraph(facets, balancer.get(), nodes);
    packLeafFacets();
    Log.info([] { return "done"; });
  }

  void Mesh::packLeafFacets()
  {
    leafPlaneNormalBlocks.clear();
    leafFacetIndices.clear();
    if (!graph)
    {
      return;
    }

    const auto entries = graph->leafGeometry.size();
    const auto zero = Zero<Float4>();
    const auto zeros = std::array<Float4, 4>{zero, zero, zero, zero};
    leafPlaneNormalBlocks.assign((entries + BlockWidth - 1) / BlockWidth, FacetNormalsBlock{zeros, zeros, zeros});
    leafFacetIndices.reserve(entries);
    for (auto entry = ASizeT{0}; entry < entries; ++entry)
    {
      const auto* node = graph->leafGeometry[entry];
      assert(dynamic_cast<const MeshGeometryNode*>(node));
      const auto index = static_cast<const MeshGeometryNode*>(node)->index;
      leafFacetIndices.push_back(static_cast<std::uint32_t>(index));

      const auto& normals = planeNormals[index];
      auto& block = leafPlaneNormalBlocks[entry / BlockWidth];
      const auto lane = entry % BlockWidth;
      for (auto c = ASizeT{0}; c < 4; ++c)
      {
        block.v0[c][lane] = normals.v0[c];
        block.v1[c][lane] = normals.v1[c];
        block.v2[c][lane] = normals.v2[c];
      }
    }
  }

  const KDTreeRoot* Mesh::getGraph() const
  {
    return graph.get();
  }

  void Mesh::clear()
  {
    graph.reset();

    for (const auto* node : nodes)
    {
      delete node;
    }
    nodes.clear();

    vertices.clear();
    vertexNormals.clear();

    facetIndices.clear();
    facets.clear();
    texCoords.clear();
    flatNormals.clear();
    smoothNormals.clear();
    planeNormals.clear();
    facetEdges.clear();
    leafPlaneNormalBlocks.clear();
    leafFacetIndices.clear();
  }

  void Mesh::setupMesh()
  {
    bounding = computeStandardMesh(vertices);
    computeFacets(vertices, facetIndices, facets);
    computeNormals(vertices, facetIndices, facets, vertexNormals, flatNormals, smoothNormals, planeNormals, facetEdges);
    graph = computeNodesAndGraph(facets, balancer.get(), nodes);
    packLeafFacets();
  }
}
