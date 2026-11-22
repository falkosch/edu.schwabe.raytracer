#pragma once

#include "../../partitioning/KDTreeBalancer.h"
#include "../../partitioning/KDTreeTraverser.h"

#include <primitives.h>
#include <vectorization.h>

#include <vector>

namespace raytracer {
  // Removes the facets without area (coincident or collinear vertices, up to rounding): they can't be hit, and their
  // normal is zero or noise. Returns the number of removed facets.
  vectorization::ASizeT removeDegenerateFacets(
      const std::vector<vectorization::Float4> &vertices, std::vector<vectorization::UInt3> &facetIndices
  ) noexcept;

  void computeFacets(
      const std::vector<vectorization::Float4> &vertices, const std::vector<vectorization::UInt3> &facetIndices,
      std::vector<primitives::Facet> &facets
  ) noexcept;

  void computeNormals(
      const std::vector<vectorization::Float4> &vertices, const std::vector<vectorization::UInt3> &facetIndices,
      const std::vector<primitives::Facet> &facets, std::vector<vectorization::Float4> &vertexNormals,
      std::vector<vectorization::Float4> &flatNormals, std::vector<primitives::FacetNormals> &smoothNormals,
      std::vector<primitives::FacetNormals> &planeNormals, std::vector<primitives::FacetEdges> &facetEdges
  ) noexcept;

  std::unique_ptr<KDTreeRoot> computeNodesAndGraph(
      const std::vector<Facet> &facets, const KDTreeBalancer *const balancer, PGeometryNodeList &nodes
  ) noexcept;

  vectorization::Float computeFacetIntersection(
      const vectorization::ASizeT intersected, const vectorization::Float4 &texCoordsAndDistance,
      const primitives::RayCast &rayCast, const std::vector<primitives::FacetTexCoords4> &texCoords,
      const std::vector<vectorization::Float4> &flatNormals, const std::vector<primitives::FacetNormals> &smoothNormals,
      const PGeometryNodeList &nodes, FacetIntersection &intersectionOut
  ) noexcept;
}
