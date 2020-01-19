#pragma once

#include "../../partitioning/KDTreeTraverser.h"
#include "../../partitioning/KDTreeBalancer.h"

#include <vectorization.h>
#include <primitives.h>

#include <vector>

namespace raytracer
{
    void computeFacets(
        const std::vector<vectorization::Float4> & vertices,
        const std::vector<vectorization::UInt3> & facetIndices,
        std::vector<primitives::Facet> & facets
    ) noexcept;

    void computeNormals(
        const std::vector<vectorization::Float4> & vertices,
        const std::vector<vectorization::UInt3> & facetIndices,
        const std::vector<primitives::Facet> & facets,
        std::vector<vectorization::Float4> & vertexNormals,
        std::vector<vectorization::Float4> & flatNormals,
        std::vector<primitives::FacetNormals> & smoothNormals,
        std::vector<primitives::FacetNormals> & planeNormals,
        std::vector<primitives::FacetEdges> & facetEdges
    ) noexcept;

    KDTreeRoot * const computeNodesAndGraph(
        const std::vector<Facet> & facets,
        const KDTreeBalancer * const balancer,
        PGeometryNodeList & nodes
    ) noexcept;

    const vectorization::Float computeFacetIntersection(
        const vectorization::ASizeT intersected,
        const vectorization::Float4 & texCoordsAndDistance,
        const primitives::Raycast & raycast,
        const std::vector<primitives::FacetTexCoords4> & texCoords,
        const std::vector<vectorization::Float4> & flatNormals,
        const std::vector<primitives::FacetNormals> & smoothNormals,
        const PGeometryNodeList & nodes,
        FacetIntersection & intersectionOut
    ) noexcept;
}
