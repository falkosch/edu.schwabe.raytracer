#pragma once

#include "../Form.h"

#include "../../partitioning/KDTreeBalancer.h"
#include "../../partitioning/KDTreeTraverser.h"

namespace raytracer {
  using namespace vectorization;
  using namespace primitives;

  struct Mesh final : Form, GeometryNodesTraverser<FacetIntersection> {
    Mesh();

    explicit Mesh(const KDTreeTraverser<FacetIntersection> *traverser, const KDTreeBalancer *balancer);

    ~Mesh() override;

    // Form interface

    AxisAlignedBoundingBox getBounding() const override;

    Float findNearestIntersection(
        const RayCast &rayCast, const FacetIntersection *const originIntersection, FacetIntersection &intersectionOut
    ) const override;

    Float findAnyIntersection(
        const RayCast &rayCast, const FacetIntersection *const originIntersection, FacetIntersection &intersectionOut
    ) const override;

    Float getIndividualIntersectionCosts() const override {
      // benchmarked intersection costs in cycles
      return 19.027224f;
    }

    // GeometryNodesTraverser interface

    Float findNearestIntersection(
        const PGeometryNodeList &geometryNodes, const RayCast &rayCast, const FacetIntersection *originIntersection,
        FacetIntersection &intersectionOut
    ) const override;

    Float findAnyIntersection(
        const PGeometryNodeList &geometryNodes, const RayCast &rayCast, const FacetIntersection *originIntersection,
        FacetIntersection &intersectionOut
    ) const override;

    // Builders and test methods

    static Mesh *buildCubeMesh();

    static Mesh *buildPlaneMesh();

    static Mesh *buildTriangleMesh();

    static Mesh *loadFromOffFile(
        const std::string &filename, bool flipNormals, const KDTreeTraverser<FacetIntersection> *traverser,
        const KDTreeBalancer *balancer
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

    PGeometryNodeList nodes;
    KDTreeRoot *graph;

    const KDTreeTraverser<FacetIntersection> *traverser;
    const KDTreeBalancer *balancer;

    void clear();

    void setupMesh();
  };
}
