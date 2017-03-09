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

        explicit Mesh(const std::string file, const bool hasRevertedNormals, const KDTreeTraverser<FacetIntersection> * const traverser, const KDTreeBalancer * const balancer);

        virtual ~Mesh();

        void buildTriangleMesh();

        void buildPlaneMesh();

        void buildCubeMesh();

        static Float4 findFacetIntersection(const Facet & trianglePlanes, const Raycast & r);

        // Form interface

        const AxisAlignedBoundingBox getBounding() const;

        const Float findNearestIntersection(const Raycast & raycast, const FacetIntersection * const originIntersection, FacetIntersection & intersectionOut) const;

        const Float findAnyIntersection(const Raycast & raycast, const FacetIntersection * const originIntersection, FacetIntersection & intersectionOut) const;

        const Float getIndividualIntersectionCosts() const
        {
            // benchmarked intersection costs in cycles
            return 19.027224f;
        }


    protected:

        std::vector<Float4> vertices;
        std::vector<UInt3> facetsIndices;

        // intermediate calculations
        std::vector<Float4> vertexNormals;
        std::vector<Facet> facets;
        std::vector<Facet> texCoords;
        std::vector<FacetNormals> smoothNormals;
        std::vector<FacetNormals> flatNormals;
        std::vector<FacetEdges> facetsEdges;

        void clear();

        void calculateTexCoordsSpherical();

        void calculateTexCoordsOrtho(const Float4 & sPlane, const Float4 & tPlane);

        const bool intersectsFacet(const ASizeT facetIndex, const Ray & ray, const Float maxDistance);

        const Float findFacetIntersection(const ASizeT facetIndex, const Ray & ray, const Float maxDistance, FacetIntersection * facetIntersectionOut);

        const bool intersects(const Ray & ray, const Float maxDistance, const ASizeT excludeId);

        const Float findNearestIntersection(const Ray & ray, const Float maxDistance, const ASizeT excludeId, FacetIntersection* facetIntersectionOut);

        void loadOff(const std::string& filename, const bool flipNormals);

        void setupMesh();

        void scaleMeshIntoCanonicalCube();

        void buildFacets();

        void calculateNormals();

    };

}
