#pragma once

#include "MeshFacetInfo.h"

#include "geometry/forms/Form.h"

#include "geometry/partitioning/KDTreeTraverser.h"
#include "geometry/partitioning/KDTreeBalancer.h"

#include <primitives/AxisAlignedBoundingBox.h>

namespace raytracer
{

    struct Mesh : public Form
    {

        Mesh();

        explicit Mesh(const KDTreeTraverser<FacetIntersection> * const traverser, const KDTreeBalancer * const balancer);

        virtual ~Mesh();

        // Form interface

        const AxisAlignedBoundingBox getBounding() const;

        const Float findNearestIntersection(const Raycast & raycast, const FacetIntersection * const originIntersection, FacetIntersection & intersectionOut) const;

        const Float findAnyIntersection(const Raycast & raycast, const FacetIntersection * const originIntersection, FacetIntersection & intersectionOut) const;

        const Float getIndividualIntersectionCosts() const
        {
            // benchmarked intersection costs in cycles
            return 19.027224f;
        }

        // Builders and test methods

        static Mesh * const buildCubeMesh();

        static Mesh * const buildPlaneMesh();

        static Mesh * const buildTriangleMesh();

        static const Float4 findFacetIntersection(const Facet & trianglePlanes, const Raycast & r);

        static Mesh * const loadFromOffFile(const std::string & filename, const bool flipNormals, const KDTreeTraverser<FacetIntersection> * const traverser, const KDTreeBalancer * const balancer);

    protected:

        AxisAlignedBoundingBox bounding;

        std::vector<UInt3> facetsIndices;
        std::vector<Float4> vertices;
        std::vector<Float4> vertexNormals;
        std::vector<Facet> facets;
        std::vector<Facet> texCoords;
        std::vector<FacetNormals> smoothNormals;
        std::vector<FacetNormals> flatNormals;
        std::vector<FacetEdges> facetsEdges;

        const KDTreeTraverser<FacetIntersection> * traverser;
        const KDTreeBalancer * balancer;

        void clear();

        void setupMesh();

    };

}
