#include "raytracing/geometry/forms/meshes/Mesh.h"
#include "../../../../stdafx.h"

#include "raytracing/geometry/forms/meshes/MeshGeometryNode.h"
#include "raytracing/geometry/forms/meshes/facets.h"
#include "raytracing/geometry/forms/meshes/textureCoordinates.h"
#include "raytracing/geometry/forms/meshes/vertices.h"

#include <cassert>
#include <fstream>
#include <ios>
#include <iostream>
#include <sstream>

namespace raytracer {
  Mesh::Mesh()
      : bounding(), vertices(), vertexNormals(), facetIndices(), facets(), texCoords(), flatNormals(), smoothNormals(),
        planeNormals(), facetEdges(), nodes(), graph(), traverser(), balancer() {
  }

  Mesh::Mesh(const KDTreeTraverser<FacetIntersection> *const traverser, const KDTreeBalancer *const balancer)
      : bounding(), vertices(), vertexNormals(), facetIndices(), facets(), texCoords(), flatNormals(), smoothNormals(),
        planeNormals(), facetEdges(), nodes(), graph(), traverser(traverser), balancer(balancer) {
  }

  Mesh::~Mesh() {
    clear();
    delete graph;
    delete traverser;
    delete balancer;
  }

  const AxisAlignedBoundingBox Mesh::getBounding() const {
    return bounding;
  }

  const Float Mesh::findNearestIntersection(
      const RayCast &rayCast, const FacetIntersection *const originIntersection, FacetIntersection &intersectionOut
  ) const {
    if (graph) {
      return traverser->findNearestIntersection(*this, *graph, rayCast, originIntersection, intersectionOut);
    }
    return findNearestIntersection(nodes, rayCast, originIntersection, intersectionOut);
  }

  const Float Mesh::findAnyIntersection(
      const RayCast &rayCast, const FacetIntersection *const originIntersection, FacetIntersection &intersectionOut
  ) const {
    if (graph) {
      return traverser->findAnyIntersection(*this, *graph, rayCast, originIntersection, intersectionOut);
    }
    return findAnyIntersection(nodes, rayCast, originIntersection, intersectionOut);
  }

  const Float Mesh::findNearestIntersection(
      const PGeometryNodeList &geometryNodes, const RayCast &rayCast, const FacetIntersection *const originIntersection,
      FacetIntersection &intersectionOut
  ) const {
    if (outOfReach(rayCast, Zero<Float>())) {
      return rayCast.maxDistance;
    }

    auto nearestTexCoordsAndDistance = Float4{rayCast.maxDistance};
    ASizeT nearestIndex{0};

    for (const auto node : geometryNodes) {
      // avoid self occlusion
      if (originIntersection && originIntersection->node == node) {
        continue;
      }

      const auto meshNode = static_cast<const MeshGeometryNode *const>(node);
      const auto index = meshNode->index;

      auto determinant = dot3v(flatNormals[index], rayCast.ray.direction);
      const auto frontfaced = isNegative(determinant);
      const auto culledBack = (!frontfaced) & backfaceCulled(rayCast);
      const auto culledFront = (!!frontfaced) & frontfaceCulled(rayCast);
      if (culledBack | culledFront) {
        continue;
      }

      // auto facetTexCoordsAndDistance = nearestIntersectionMoeller(
      //     facets[index].v0,
      //     facetEdges[index],
      //     rayCast.ray,
      //     nearestTexCoordsAndDistance
      //);
      auto facetTexCoordsAndDistance =
          nearestIntersectionHavel(planeNormals[index], rayCast, zzzz(nearestTexCoordsAndDistance));

      if (z(facetTexCoordsAndDistance < nearestTexCoordsAndDistance)) {
        nearestTexCoordsAndDistance = facetTexCoordsAndDistance;
        nearestIndex = index;
      }
    }

    return computeFacetIntersection(
        nearestIndex, nearestTexCoordsAndDistance, rayCast, texCoords, flatNormals, smoothNormals, nodes,
        intersectionOut
    );
  }

  // Finds any intersection of a Ray within a geometry.
  const Float Mesh::findAnyIntersection(
      const PGeometryNodeList &geometryNodes, const RayCast &rayCast, const FacetIntersection *const originIntersection,
      FacetIntersection &intersectionOut
  ) const {
    if (outOfReach(rayCast, Zero<Float>())) {
      return rayCast.maxDistance;
    }

    const auto maxDistance = Float4(rayCast.maxDistance);

    for (const auto node : geometryNodes) {
      // avoid self occlusion
      if (originIntersection && originIntersection->node == node) {
        continue;
      }

      const auto meshNode = static_cast<const MeshGeometryNode *const>(node);
      const auto index = meshNode->index;

      auto determinant = dot3v(flatNormals[index], rayCast.ray.direction);
      const auto frontfaced = isNegative(determinant);
      const auto culledBack = (!frontfaced) & backfaceCulled(rayCast);
      const auto culledFront = (!!frontfaced) & frontfaceCulled(rayCast);
      if (culledBack | culledFront) {
        continue;
      }

      // auto facetTexCoordsAndDistance = nearestIntersectionMoeller(
      //     facets[index].v0,
      //     facetEdges[index],
      //     rayCast.ray,
      //     maxDistance
      //);
      auto facetTexCoordsAndDistance = nearestIntersectionHavel(planeNormals[index], rayCast, zzzz(maxDistance));

      // is distance in t more near than the distance in preserved d
      if (z(facetTexCoordsAndDistance < maxDistance)) {
        return computeFacetIntersection(
            index, facetTexCoordsAndDistance, rayCast, texCoords, flatNormals, smoothNormals, nodes, intersectionOut
        );
      }
    }

    return rayCast.maxDistance;
  }

  Mesh *const Mesh::buildCubeMesh() {
    const auto mesh = new Mesh();

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

  Mesh *const Mesh::buildPlaneMesh() {
    const auto mesh = new Mesh();

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

  Mesh *const Mesh::buildTriangleMesh() {
    const auto mesh = new Mesh();

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

  const bool readMeshFileContent(std::ifstream file, std::string &stringOut) {
    if (file.is_open()) {
      stringOut = std::string{std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
      file.close();
      return true;
    }
    return false;
  }

  void readNextNonEmptyLineInOFF(std::istringstream &fileStream, std::string &line) {
    line = "";
    while (line.empty()) {
      std::getline(fileStream, line);
    }
  }

  const bool readAndCheckHeaderInOFF(std::istringstream &fileStream, std::string &line) {
    while (std::getline(fileStream, line)) {
      if (line == "OFF" || line == "OFF\r") {
        return true;
      }
    }
    return false;
  }

  Mesh *const Mesh::loadFromOffFile(
      const std::string &filename, const bool flipNormals, const KDTreeTraverser<FacetIntersection> *const traverser,
      const KDTreeBalancer *const balancer
  ) {
    auto mesh = new Mesh(traverser, balancer);
    if (filename.empty()) {
      std::cerr << "File name is empty" << std::endl;
      return mesh;
    }

    std::string fileContent;
    if (!readMeshFileContent(std::ifstream{filename}, fileContent)) {
      std::cerr << "Failed to open " << filename << std::endl;
      return mesh;
    }
    std::cout << "Loaded " << fileContent.size() << " bytes from file " << filename << std::endl;

    std::istringstream fileStream{fileContent};
    std::string line;
    if (!readAndCheckHeaderInOFF(fileStream, line)) {
      std::cerr << "File " << filename << " is not an OFF-file" << std::endl;
      return mesh;
    }

    readNextNonEmptyLineInOFF(fileStream, line);

    // read number of vertices and faces
    ASizeT verticesCount, facetsCount;
    std::istringstream{line} >> verticesCount >> facetsCount;
    assert(verticesCount > 0);
    assert(facetsCount > 0);

    // read vertices
    mesh->vertices.resize(verticesCount);
    for (ASizeT i{0}; i < verticesCount; i++) {
      readNextNonEmptyLineInOFF(fileStream, line);

      Float x, y, z;
      std::istringstream{line} >> x >> y >> z;
      mesh->vertices[i] = Float4{x, y, z, One<Float>()};
    }

    // read faces
    mesh->facetIndices.resize(facetsCount);
    for (ASizeT i{0}; i < facetsCount; i++) {
      readNextNonEmptyLineInOFF(fileStream, line);

      std::istringstream lineStream{line};

      ASizeT faceVerticesCount;
      lineStream >> faceVerticesCount;
      if (faceVerticesCount != 3) {
        std::cerr << filename << " is corrupted!" << std::endl;
        return mesh;
      }

      UInt v1, v2, v3;
      lineStream >> v1 >> v2 >> v3;
      if (flipNormals) {
        mesh->facetIndices[i] = UInt3{v1, v3, v2};
      } else {
        mesh->facetIndices[i] = UInt3{v1, v2, v3};
      }
    }

    mesh->setupMesh();
    computeTexCoordsSpherical(mesh->facetIndices, mesh->facets, mesh->texCoords);

    std::cout << "loaded " << filename << ": " << verticesCount << " vertices, " << facetsCount << " faces"
              << std::endl;

    return mesh;
  }

  void Mesh::clear() {
    vertices.clear();
    vertexNormals.clear();

    facetIndices.clear();
    facets.clear();
    texCoords.clear();
    flatNormals.clear();
    smoothNormals.clear();
    planeNormals.clear();
    facetEdges.clear();
  }

  void Mesh::setupMesh() {
    bounding = computeStandardMesh(vertices);
    computeFacets(vertices, facetIndices, facets);
    computeNormals(vertices, facetIndices, facets, vertexNormals, flatNormals, smoothNormals, planeNormals, facetEdges);
    graph = computeNodesAndGraph(facets, balancer, nodes);
  }
}
