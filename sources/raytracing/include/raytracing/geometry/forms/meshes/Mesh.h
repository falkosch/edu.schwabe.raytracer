#pragma once

#include "../Form.h"

#include "../../partitioning/KDTreeTraverser.h"
#include "../../partitioning/KDTreeBalancer.h"

namespace raytracer
{
    using namespace vectorization;
    using namespace primitives;

    struct Mesh : public Form, GeometryNodesTraverser<FacetIntersection>
    {
        Mesh();

        explicit Mesh(const KDTreeTraverser<FacetIntersection> * const traverser, const KDTreeBalancer * const balancer);

        virtual ~Mesh();

        // Form interface

        const AxisAlignedBoundingBox getBounding() const;

        const Float findNearestIntersection(const Raycast & raycast, const FacetIntersection * const originIntersection, FacetIntersection & intersectionOut) const;

        const Float findAnyIntersection(const Raycast & raycast, const FacetIntersection * const originIntersection, FacetIntersection & intersectionOut) const;

        const Float getIndividualIntersectionCosts() const {
            // benchmarked intersection costs in cycles
            return 19.027224f;
        }

        // GeometryNodesTraverser interface

        const Float findNearestIntersection(const PGeometryNodeList & geometryNodes, const Raycast & raycast, const FacetIntersection * const originIntersection, FacetIntersection & intersectionOut) const;

        const Float findAnyIntersection(const PGeometryNodeList & geometryNodes, const Raycast & raycast, const FacetIntersection * const originIntersection, FacetIntersection & intersectionOut) const;

        // Builders and test methods

        static Mesh * const buildCubeMesh();

        static Mesh * const buildPlaneMesh();

        static Mesh * const buildTriangleMesh();

        static Mesh * const loadFromOffFile(const std::string & filename, const bool flipNormals, const KDTreeTraverser<FacetIntersection> * const traverser, const KDTreeBalancer * const balancer);

    protected:

        AxisAlignedBoundingBox bounding;

        // each has size equal to the count of vertices
        std::vector<Float4> vertices; // raw
        std::vector<Float4> vertexNormals;

        // each has size equal to the count of facetsIndices
        std::vector<UInt3> facetIndices; // per facet indices to vertices
        std::vector<Facet> facets;
        std::vector<Facet> texCoords;
        std::vector<Float4> flatNormals; // per facet
        std::vector<FacetNormals> smoothNormals;
        // intermediate for havel intersection test
        std::vector<FacetNormals> planeNormals;
        // intermediate for moeller intersection test
        std::vector<FacetEdges> facetEdges;

        PGeometryNodeList nodes;
        KDTreeRoot * graph;

        const KDTreeTraverser<FacetIntersection> * traverser;
        const KDTreeBalancer * balancer;

        void clear();

        void setupMesh();
    };
}
