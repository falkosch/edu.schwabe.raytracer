#include "raytracing/geometry/forms/meshes/facets.h"
#include "../../../../stdafx.h"

#include "raytracing/geometry/forms/meshes/MeshGeometryNode.h"

namespace raytracer {
  void computeFacets(
      const std::vector<Float4> &vertices, const std::vector<UInt3> &facetIndices, std::vector<Facet> &facets
  ) noexcept {
    facets.resize(facetIndices.size());

#pragma omp parallel for
    for (auto i = int{0}; i < static_cast<int>(facetIndices.size()); i++) {
      auto index = static_cast<ASizeT>(i);
      auto &indices = facetIndices[index];
      facets[index] = Facet{vertices[x(indices)], vertices[y(indices)], vertices[z(indices)]};
    }
  }

  void computeNormals(
      const std::vector<Float4> &vertices, const std::vector<UInt3> &facetIndices, const std::vector<Facet> &facets,
      std::vector<Float4> &vertexNormals, std::vector<Float4> &flatNormals, std::vector<FacetNormals> &smoothNormals,
      std::vector<FacetNormals> &planeNormals, std::vector<FacetEdges> &facetEdges
  ) noexcept {
    vertexNormals.resize(vertices.size());

    auto facetsCount = facets.size();
    flatNormals.resize(facetsCount);
    smoothNormals.resize(facetsCount);
    planeNormals.resize(facetsCount);
    facetEdges.resize(facetsCount);

#pragma omp parallel for
    for (auto i = int{0}; i < static_cast<int>(facets.size()); i++) {
      auto index = static_cast<ASizeT>(i);

      // edges in counter-clockwise order
      auto &facet = facets[index];
      auto edge1 = facet.v1 - facet.v0;
      auto edge2 = facet.v2 - facet.v0;
      facetEdges[index] = FacetEdges{edge1, edge2};

      auto planeNormal0 = cross3(edge1, edge2);
      auto planeNormal1 = cross3(edge2, planeNormal0);
      auto planeNormal2 = cross3(planeNormal0, edge1);
      planeNormals[index] = FacetNormals{
          replaceW(planeNormal0, -dot3(facet.v0, planeNormal0)), replaceW(planeNormal1, -dot3(facet.v0, planeNormal1)),
          replaceW(planeNormal2, -dot3(facet.v0, planeNormal2))
      };
      planeNormals[index].v1 /= dotv(planeNormals[index].v1, facet.v1);
      planeNormals[index].v2 /= dotv(planeNormals[index].v2, facet.v2);

      auto flatNormal = normalize3(planeNormal0);
      flatNormals[index] = flatNormal;

      // calculate weighted face normal part for the vertex normals
      auto l0 = length3v(edge1);
      auto l1 = length3v(facet.v1 - facet.v2);
      auto l2 = length3v(edge2);
      const FacetNormals normalsWeighted{flatNormal / (l0 * l2), flatNormal / (l1 * l0), flatNormal / (l2 * l1)};

#pragma omp critical
      {
        auto &indices = facetIndices[index];
        vertexNormals[x(indices)] += normalsWeighted.v0;
        vertexNormals[y(indices)] += normalsWeighted.v1;
        vertexNormals[z(indices)] += normalsWeighted.v2;
      }
    }

    // normalize vertex-normals
#pragma omp parallel for
    for (auto i = int{0}; i < static_cast<int>(vertices.size()); i++) {
      auto index = static_cast<ASizeT>(i);
      vertexNormals[index] = normalize3(vertexNormals[index]);
    }

    // build smooth normals
#pragma omp parallel for
    for (auto i = int{0}; i < static_cast<int>(facets.size()); i++) {
      auto index = static_cast<ASizeT>(i);
      auto &indices = facetIndices[index];
      auto &normalV0 = vertexNormals[x(indices)];
      auto &normalV1 = vertexNormals[y(indices)];
      auto &normalV2 = vertexNormals[z(indices)];
      auto &flatNormal = flatNormals[index];

      smoothNormals[index] = FacetNormals{
          select(dot3(flatNormal, normalV0) < Sin45<Float>(), flatNormal, normalV0),
          select(dot3(flatNormal, normalV1) < Sin45<Float>(), flatNormal, normalV1),
          select(dot3(flatNormal, normalV2) < Sin45<Float>(), flatNormal, normalV2)
      };
    }
  }

  KDTreeRoot *const computeNodesAndGraph(
      const std::vector<Facet> &facets, const KDTreeBalancer *const balancer, PGeometryNodeList &nodes
  ) noexcept {
    nodes.resize(facets.size());

#pragma omp parallel for
    for (auto i = int{0}; i < static_cast<int>(facets.size()); i++) {
      auto index = static_cast<ASizeT>(i);
      nodes[index] = new MeshGeometryNode{index, facets[index]};
    }

    if (balancer) {
      return balancer->build(nodes);
    }
    return nullptr;
  }

  const Float computeFacetIntersection(
      const ASizeT index, const Float4 &uvAndDistance, const RayCast &rayCast,
      const std::vector<FacetTexCoords4> &texCoords, const std::vector<Float4> &flatNormals,
      const std::vector<FacetNormals> &smoothNormals, const PGeometryNodeList &nodes, FacetIntersection &intersectionOut
  ) noexcept {
    auto distance = zzzz(uvAndDistance);

    // missed intersections do not require any computations
    if (anyTrue(outOfReach(rayCast, distance))) {
      return rayCast.maxDistance;
    }

    auto vertex = rayCast.ray.origin + distance * rayCast.ray.direction;
    intersectionOut.msVertex = vertex;
    intersectionOut.vertex = vertex;

    auto &surfaceNormal = flatNormals[index];
    intersectionOut.msSurfaceNormal = surfaceNormal;
    intersectionOut.surfaceNormal = surfaceNormal;

    intersectionOut.smoothedNormal = normalize3(baryCenter(uvAndDistance, smoothNormals[index]));
    intersectionOut.texCoords = xy_zw(baryCenter(uvAndDistance, texCoords[index]), OneW<Float4>());
    intersectionOut.node = nodes[index];

    return z(uvAndDistance);
  }
}