#pragma once

#include "MeshGeometryNode.h"

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

        static const Float4 findFacetIntersection(const Facet & trianglePlanes, const Raycast & r);

        static Mesh * const loadFromOffFile(const std::string & filename, const bool flipNormals, const KDTreeTraverser<FacetIntersection> * const traverser, const KDTreeBalancer * const balancer);

    protected:

        AxisAlignedBoundingBox bounding;

        std::vector<UInt3> facetsIndices; // raw
        std::vector<Float4> vertices; // raw
        std::vector<Float4> vertexNormals;
        std::vector<Facet> facets;
        std::vector<Facet> texCoords;
        std::vector<Float4> flatNormals; // per facet
        std::vector<FacetNormals> smoothNormals;
        std::vector<FacetEdges> facetsEdges; // intermediate for intersection test

        PGeometryNodeList nodes;
        KDTreeRoot * graph;

        const KDTreeTraverser<FacetIntersection> * traverser;
        const KDTreeBalancer * balancer;

        void clear();

        void setupMesh();
    };
}
