#pragma once

#include "../Form.h"

#include "../../partitioning/KDTreeBalancer.h"
#include "../../partitioning/KDTreeTraverser.h"

#include <cstdint>
#include <memory>
#include <vector>

namespace raytracer {
  using namespace vectorization;
  using namespace primitives;

  struct Mesh final : Form, GeometryNodesTraverser<FacetIntersection> {
    Mesh();

    Mesh(std::unique_ptr<const KDTreeTraverser<FacetIntersection>> traverser, std::unique_ptr<const KDTreeBalancer> balancer);

    ~Mesh() override;

    // Form interface

    AxisAlignedBoundingBox getBounding() const override;

    Float findNearestIntersection(
        const RayCast &rayCast, const FacetIntersection *const originIntersection, FacetIntersection &intersectionOut
    ) const override;

    Float findAnyIntersection(
        const RayCast &rayCast, const FacetIntersection *const originIntersection, FacetIntersection &intersectionOut
    ) const override;

    ASizeT getFaceCount() const override;

    Float getIndividualIntersectionCosts() const override {
      return 9.377283f;
    }

    // GeometryNodesTraverser interface

    Float findNearestIntersection(
        GeometryNodeSpan geometryNodes, const RayCast &rayCast, const FacetIntersection *originIntersection,
        FacetIntersection &intersectionOut
    ) const override;

    Float findAnyIntersection(
        GeometryNodeSpan geometryNodes, const RayCast &rayCast, const FacetIntersection *originIntersection,
        FacetIntersection &intersectionOut
    ) const override;

    void setTreeBalancer(std::unique_ptr<const KDTreeBalancer> balancer);

    // KD-tree over the facets; nullptr if the mesh was built without a balancer
    const KDTreeRoot *getGraph() const;

    // Builders and test methods. With a traverser and a balancer, the mesh intersects through a KD-tree over its facets
    // as a loaded one does; without them, it tests every facet.

    static std::unique_ptr<Mesh> buildCubeMesh(
        std::unique_ptr<const KDTreeTraverser<FacetIntersection>> traverser = nullptr,
        std::unique_ptr<const KDTreeBalancer> balancer = nullptr
    );

    static std::unique_ptr<Mesh> buildPlaneMesh(
        std::unique_ptr<const KDTreeTraverser<FacetIntersection>> traverser = nullptr,
        std::unique_ptr<const KDTreeBalancer> balancer = nullptr
    );

    static std::unique_ptr<Mesh> buildTriangleMesh(
        std::unique_ptr<const KDTreeTraverser<FacetIntersection>> traverser = nullptr,
        std::unique_ptr<const KDTreeBalancer> balancer = nullptr
    );

    static std::unique_ptr<Mesh> loadFromOffFile(
        const std::string &filename, bool flipNormals, std::unique_ptr<const KDTreeTraverser<FacetIntersection>> traverser,
        std::unique_ptr<const KDTreeBalancer> balancer
    );

  protected:
    AxisAlignedBoundingBox bounding;

    // each has a size equal to the count of vertices
    std::vector<Float4> vertices; // raw
    std::vector<Float4> vertexNormals;

    // each has size equal to the count of facetsIndices
    std::vector<UInt3> facetIndices; // per facet indices to vertices
    std::vector<Facet> facets;
    std::vector<Facet> texCoords;
    std::vector<Float4> flatNormals; // per facet
    std::vector<FacetNormals> smoothNormals;
    // intermediate for Havel intersection test
    std::vector<FacetNormals> planeNormals;
    // intermediate for Möller intersection test
    std::vector<FacetEdges> facetEdges;

    // The elements of graph->leafGeometry in the same order, so a leaf is a range of entries: plane normals in blocks of
    // FacetLanes::SIZE for the batch Havel test (lane i of block b is entry FacetLanes::SIZE * b + i), and the facet
    // index of every entry
    std::vector<FacetNormalsBlock> leafPlaneNormalBlocks;
    std::vector<std::uint32_t> leafFacetIndices;

    PGeometryNodeList nodes;
    std::unique_ptr<KDTreeRoot> graph;

    std::unique_ptr<const KDTreeTraverser<FacetIntersection>> traverser;
    std::unique_ptr<const KDTreeBalancer> balancer;

    void clear();

    void setupMesh();

    void packLeafFacets();
  };
}
