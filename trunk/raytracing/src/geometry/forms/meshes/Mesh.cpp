#include "../../../stdafx.h"

#include "geometry/forms/meshes/Mesh.h"

#include <sstream>
#include <fstream>

namespace raytracer
{

#pragma region privates
    //{privates

    const AxisAlignedBoundingBox computeBounding(std::vector<Float4> & vertices)
    {
        AxisAlignedBoundingBox allBounding = AxisAlignedBoundingBox();

#pragma omp parallel
        {
            AxisAlignedBoundingBox bounding = AxisAlignedBoundingBox();
            const int verticesCount = int(vertices.size());

#pragma omp for
            for (int i = 0; i < verticesCount; i++)
            {
                bounding = extendBy(bounding, vertices[ASizeT(i)]);
            }

#pragma omp critical
            {
                allBounding = extendBy(allBounding, bounding);
            }
        }

        return allBounding;
    }

    void computeFacets(
        std::vector<UInt3> & facetsIndices,
        std::vector<Float4> & vertices,
        std::vector<Facet> & facets)
    {
        const ASizeT facetsCount = facetsIndices.size();
        facets.resize(facetsCount);

        const int facetsCounti = int(facetsCount);
#pragma omp parallel for
        for (int i = 0; i < facetsCounti; i++)
        {
            const ASizeT j = ASizeT(i);
            const UInt3 facetIndices = facetsIndices[j];
            facets[j] = Facet(vertices[x(facetIndices)], vertices[y(facetIndices)], vertices[z(facetIndices)]);
        }
    }

    void computeNormals(
        std::vector<UInt3> & facetsIndices,
        std::vector<Float4> & vertices,
        std::vector<Float4> & vertexNormals,
        std::vector<Facet> & facets,
        std::vector<FacetNormals> & smoothNormals,
        std::vector<FacetNormals> & flatNormals,
        std::vector<FacetEdges> & facetsEdges)
    {
        const ASizeT facetsCount = facets.size();
        facetsEdges.resize(facetsCount);
        flatNormals.resize(facetsCount);
        smoothNormals.resize(facetsCount);

        const ASizeT verticesCount = vertices.size();
        vertexNormals.resize(verticesCount);

        const int facetsCounti = int(facetsCount);

#pragma omp parallel for
        for (int i = 0; i < facetsCounti; i++)
        {
            const ASizeT j = ASizeT(i);
            const Facet facet = facets[j];

            // edges in counter-clockwise order
            const Float4 edge0 = facet.v0 - facet.v1;
            const Float4 edge1 = facet.v1 - facet.v2;
            const Float4 edge2 = facet.v2 - facet.v0;
            const Float4 l0 = length3v(edge0);
            const Float4 l1 = length3v(edge1);
            const Float4 l2 = length3v(edge2);
            const Float4 flatNormal = normalize3(cross3(edge0, edge1));

            // precomputes for intersection test
            facetsEdges[j] = FacetEdges(-edge0, edge2);

            // set face normal for each face vertex
            flatNormals[j] = Facet(flatNormal);

            // calculate weighted face normal part for the vertex normals
            const Facet normalsWeighted = Facet(
                flatNormal / (l0 * l2),
                flatNormal / (l1 * l0),
                flatNormal / (l2 * l1));

#pragma omp critical
            {
                const UInt3 & facetIndices = facetsIndices[j];
                vertexNormals[x(facetIndices)] += normalsWeighted.v0;
                vertexNormals[y(facetIndices)] += normalsWeighted.v1;
                vertexNormals[z(facetIndices)] += normalsWeighted.v2;
            }
        }

        // normalize vertex-normals
#pragma omp parallel
        {
            const int verticesCounti = int(verticesCount);

#pragma omp for nowait
            for (int i = 0; i < verticesCounti; i++)
            {
                const ASizeT j = ASizeT(i);
                vertexNormals[j] = normalize3(vertexNormals[j]);
            }
        }

        // build smooth normals
#pragma omp parallel for
        for (int i = 0; i < facetsCounti; i++)
        {
            const ASizeT j = ASizeT(i);
            const UInt3 & facetIndices = facetsIndices[j];
            smoothNormals[j] = Facet(vertexNormals[x(facetIndices)], vertexNormals[y(facetIndices)], vertexNormals[z(facetIndices)]);

            // flatNormal comparison for normal adjustment at hard edges
            //const Float4 flatNormal = flatNormals[j].v0;
            //if (dot(normals->v0, flatNormal) <= glm::one_over_root_two<Float>())
            //    normals->v0 = flatNormal;
            //if (dot(normals->v1, flatNormal) <= glm::one_over_root_two<Float>())
            //    normals->v1 = flatNormal;
            //if (dot(normals->v2, flatNormal) <= glm::one_over_root_two<Float>())
            //    normals->v2 = flatNormal;
        }
    }

    void computeStandardMesh(AxisAlignedBoundingBox & bounding, std::vector<Float4> & vertices)
    {
        const AxisAlignedBoundingBox prebounds = computeBounding(vertices);
        const Float4 center = Half<Float4>() * (prebounds.maximum + prebounds.minimum);
        const Float4 extends = prebounds.maximum - prebounds.minimum;
        const Float4 scale = reciprocal(min3v(blendMasked(
            extends,
            max3v(extends),
            extends == Zero<Float4>())));

        // scale and translate
        const int verticesCount = int(vertices.size());
#pragma omp parallel for
        for (int i = 0; i < verticesCount; i++)
        {
            const ASizeT j = ASizeT(i);
            vertices[j] = replaceW((vertices[j] - center) * scale, One<Float>());
        }

        // save final bounding
        bounding = computeBounding(vertices);
    }

    void computeTexCoordsSpherical(
        std::vector<UInt3> & facetsIndices,
        std::vector<Facet> & facets,
        std::vector<Facet> & texCoords)
    {
        const ASizeT facetsCount = facetsIndices.size();
        const int facetsCounti = (int)facetsCount;

        // calculate tex-coords for each vertex-normal
        texCoords.resize(facetsCount);
#pragma omp parallel for
        for (int i = 0; i < facetsCounti; i++)
        {
            const ASizeT j = ASizeT(i);
            Facet facet = facets[j];

            // normalize vertices, they are used as normals for determining the texcoords
            // "- Common::ONE_W" elimates w-coord, which is always 1.0f for vertices
            facet.v0 = normalize3(facet.v0);
            facet.v1 = normalize3(facet.v1);
            facet.v2 = normalize3(facet.v2);

            // calculate latitude coords
            Facet *facetTexCoords = &texCoords[j];


            facetTexCoords->v0 = Float4((vectorization::atan2(x(facet.v0), z(facet.v0)) + Pi<Float>()) * RadianToUniform<Float>(),
                vectorization::acos(y(-facet.v0)) * ReciprocalPi<Float>());

            facetTexCoords->v1 = Float4((vectorization::atan2(x(facet.v1), z(facet.v1)) + Pi<Float>()) * RadianToUniform<Float>(),
                vectorization::acos(y(-facet.v1)) * ReciprocalPi<Float>());

            facetTexCoords->v2 = Float4((vectorization::atan2(x(facet.v2), z(facet.v2)) + Pi<Float>()) * RadianToUniform<Float>(),
                vectorization::acos(y(-facet.v2)) * ReciprocalPi<Float>());

            // general idea: http://www.biederman.net/leslie/terranim8or/sphere_mapping.htm
            // the distortion happens on the negative z-side at the transition of positive to negative x-coords
            // so we're determine min, mid and max to check where a triangle is positioned
            if (z(facet.v0) > 0.0f || z(facet.v1) > 0.0f || z(facet.v2) > 0.0f)
                continue;

            // search min, max and mid points of facet
            Float4 *min, *mid, *max;
            min = mid = max = &facet.v0;
            Float4 *minUV, *midUV, *maxUV;
            minUV = midUV = maxUV = &facetTexCoords->v0;
            if (x(facet.v1) < x(facet.v0))
            {
                if (x(facet.v2) < x(facet.v1))
                {
                    mid = &facet.v1;
                    midUV = &facetTexCoords->v1;
                    min = &facet.v2;
                    minUV = &facetTexCoords->v2;
                }
                else if (x(facet.v2) < x(facet.v0))
                {
                    min = &facet.v1;
                    minUV = &facetTexCoords->v1;
                    mid = &facet.v2;
                    midUV = &facetTexCoords->v2;
                }
                else
                {
                    min = &facet.v1;
                    minUV = &facetTexCoords->v1;
                    max = &facet.v2;
                    maxUV = &facetTexCoords->v2;
                }
            }
            else if (x(facet.v1) >= x(facet.v0))
            {
                if (x(facet.v2) >= x(facet.v1))
                {
                    mid = &facet.v1;
                    midUV = &facetTexCoords->v1;
                    max = &facet.v2;
                    maxUV = &facetTexCoords->v2;
                }
                else if (x(facet.v2) >= x(facet.v0))
                {
                    max = &facet.v1;
                    maxUV = &facetTexCoords->v1;
                    mid = &facet.v2;
                    midUV = &facetTexCoords->v2;
                }
                else
                {
                    max = &facet.v1;
                    maxUV = &facetTexCoords->v1;
                    min = &facet.v2;
                    minUV = &facetTexCoords->v2;
                }
            }

            // case 1: facet overlaps x-axis; min->x is negative, max->x is positive
            if (x(*min) < 0.0f && x(*max) > 0.0f)
            {
                // subcase for easier fixing: if mid->x is not negative, only minUV needs to be fixed
                if (x(*mid) >= 0.0f)
                    *minUV += vectorization::wzyx(OneW<Float4>());
                else // subcase: mid is on the negative side, only maxUV needs to be fixed
                    *maxUV -= vectorization::wzyx(OneW<Float4>());
            }
            // case 2: facet touches the origin of the x-axis with max-vertex, but doesn't overlap
            else if (x(*max) == 0.0f)
            {
                if (x(*mid) == 0.0f)
                    *minUV += vectorization::wzyx(OneW<Float4>());
                else
                    *maxUV += vectorization::wzyx(OneW<Float4>());
            }
        }
    }

    void computeTexCoordsOrtho(
        std::vector<UInt3> & facetsIndices,
        std::vector<Facet> & facets,
        std::vector<Facet> & texCoords,
        const Float4 & sPlane,
        const Float4 & tPlane)
    {
        const ASizeT facetsCount = facetsIndices.size();
        texCoords.resize(facetsCount);

        const int facetsCounti = int(facetsCount);
#pragma omp parallel for
        for (int i = 0; i < facetsCounti; i++)
        {
            const ASizeT j = ASizeT(i);
            const Facet facet = facets[j];
            texCoords[j] = Facet(
                Float4(dot(facet.v0, sPlane), dot(facet.v0, tPlane)),
                Float4(dot(facet.v1, sPlane), dot(facet.v1, tPlane)),
                Float4(dot(facet.v2, sPlane), dot(facet.v2, tPlane)));
        }
    }

    const Float4 computeIntersection(const Float4 & facetV0, const FacetEdges & facetEdges, const Ray & ray, const Float4 & maxDistance)
    {
        // intersection test based on Fast, Minimum Storage Ray/Triangle Intersection
        // http://www.graphics.cornell.edu/pubs/1997/MT97.pdf

        // calculate direction from v0 to ray.origin
        const Float4 tvec = ray.origin - facetV0;

        // calculate determinant (also part of the barycentric-U-coord)
        const Float4 pvec = cross3(ray.direction, facetEdges.v1);
        const Float4 det = reciprocal(dotv(facetEdges.v0, pvec));

        // triangle bound check on U
        const Float4 u = dotv(tvec, pvec) * det;
        if (allTrue(u < Zero<Float4>() | u > One<Float4>())) {
            return maxDistance;
        }

        // triangle bound check on V
        const Float4 qvec = cross3(tvec, facetEdges.v0);
        const Float4 v = dotv(ray.direction, qvec) * det;
        if (allTrue(v < Zero<Float4>() | (u + v) > One<Float4>())) {
            return maxDistance;
        }

        // calculate t (in "orig + t * dir" == "ray.origin + t * ray.direction")
        const Float4 t = dotv(facetEdges.v1, qvec) * det;
        return x_yzw(u, xy_zw(v, t));
    }

    //}
#pragma endregion

    Mesh::Mesh()
        :
        bounding(),
        facetsIndices(),
        vertices(),
        vertexNormals(),
        facets(),
        texCoords(),
        smoothNormals(),
        flatNormals(),
        facetsEdges(),
        balancer(),
        traverser()
    { }

    Mesh::Mesh(const KDTreeTraverser<FacetIntersection> * const traverser, const KDTreeBalancer * const balancer)
        :
        bounding(),
        facetsIndices(),
        vertices(),
        vertexNormals(),
        facets(),
        texCoords(),
        smoothNormals(),
        flatNormals(),
        facetsEdges(),
        balancer(balancer),
        traverser(traverser)
    { }

    Mesh::~Mesh()
    {
        clear();
        if (traverser)
        {
            delete traverser;
            traverser = NULL;
        }
        if (balancer)
        {
            delete balancer;
            balancer = NULL;
        }
    }

#pragma region Form interface
    //{Form interface

    const AxisAlignedBoundingBox Mesh::getBounding() const
    {
        return bounding;
    }

    const Float Mesh::findNearestIntersection(const Raycast & raycast, const FacetIntersection * const originIntersection, FacetIntersection & intersectionOut) const
    {
        if (outOfReach(raycast, Zero<Float>()))
        {
            return raycast.maxDistance;
        }

        Float4 d = Float4(raycast.maxDistance);
        ASizeT intersected = Zero<ASizeT>();

        const ASizeT facetsCount = facets.size();
        for (ASizeT i = Zero<ASizeT>(); i < facetsCount; i++)
        {
            if (originIntersection && (ASizeT(originIntersection->node) == ASizeT(&facets[i]))) {
                continue;
            }

            const Float4 t = computeIntersection(facets[i].v0, facetsEdges[i], raycast.ray, d);
            if (z(t < d)) // is distance in t more near than the distance in preserved d
            {
                d = t;
                intersected = i;
            }
        }

        // was there an intersection?
        if (z(outOfReach(raycast, d))) {
            // no intersection
            return raycast.maxDistance;
        }

        const Float4 vertex = raycast.ray.origin + zzzz(d) * raycast.ray.direction;
        const Float4 surfaceNormal = flatNormals[intersected].v0;
        const Facet & vNormals = smoothNormals[intersected];
        const Facet & vTexCoords = texCoords[intersected];

        // T(u, v) = (1 - u - v) * V0 + u * V1 + v * V2
        const Float4 u = xxxx(d), v = yyyy(d);
        const Float4 barycenterNormal = vNormals.v0 + u * (vNormals.v1 - vNormals.v0) + v * (vNormals.v2 - vNormals.v0);
        const Float4 barycenterTex = vTexCoords.v0 + u * (vTexCoords.v1 - vTexCoords.v0) + v * (vTexCoords.v2 - vTexCoords.v0);

        intersectionOut.msVertex = vertex;
        intersectionOut.msSurfaceNormal = surfaceNormal;
        intersectionOut.vertex = vertex;
        intersectionOut.surfaceNormal = surfaceNormal;
        intersectionOut.smoothedNormal = normalize3(barycenterNormal);
        intersectionOut.texCoords = xy_zw(barycenterTex, OneW<Float4>());
        //intersectionOut.node = &facets[intersected];

        return z(d);
    }

    const Float Mesh::findAnyIntersection(const Raycast & r, const FacetIntersection * const originIntersection, FacetIntersection & intersectionOut) const
    {
        return findNearestIntersection(r, originIntersection, intersectionOut);
    }

    //}
#pragma endregion

#pragma region Builders and test methods
    //{Builders and test methods

    Mesh * const Mesh::buildCubeMesh()
    {
        Mesh * const mesh = new Mesh();

        mesh->vertices.reserve(8);
        mesh->vertices.push_back(Float4(-1.0f, -1.0f, -1.0f, 1.0f));
        mesh->vertices.push_back(Float4(-1.0f, -1.0f, 1.0f, 1.0f));
        mesh->vertices.push_back(Float4(1.0f, -1.0f, 1.0f, 1.0f));
        mesh->vertices.push_back(Float4(1.0f, -1.0f, -1.0f, 1.0f));
        mesh->vertices.push_back(Float4(-1.0f, 1.0f, -1.0f, 1.0f));
        mesh->vertices.push_back(Float4(-1.0f, 1.0f, 1.0f, 1.0f));
        mesh->vertices.push_back(Float4(1.0f, 1.0f, 1.0f, 1.0f));
        mesh->vertices.push_back(Float4(1.0f, 1.0f, -1.0f, 1.0f));

        mesh->facetsIndices.reserve(12);
        // 2 x bottom facets
        mesh->facetsIndices.push_back(UInt3(0, 2, 1));
        mesh->facetsIndices.push_back(UInt3(2, 0, 3));
        // 2 x top facets
        mesh->facetsIndices.push_back(UInt3(4, 5, 6));
        mesh->facetsIndices.push_back(UInt3(6, 7, 4));
        // 2 x left facets
        mesh->facetsIndices.push_back(UInt3(0, 1, 5));
        mesh->facetsIndices.push_back(UInt3(5, 4, 0));
        // 2 x front facets
        mesh->facetsIndices.push_back(UInt3(1, 2, 6));
        mesh->facetsIndices.push_back(UInt3(6, 5, 1));
        // 2 x right facets
        mesh->facetsIndices.push_back(UInt3(2, 3, 7));
        mesh->facetsIndices.push_back(UInt3(7, 6, 2));
        // 2 x back facets
        mesh->facetsIndices.push_back(UInt3(3, 0, 4));
        mesh->facetsIndices.push_back(UInt3(4, 7, 3));

        mesh->setupMesh();
        computeTexCoordsOrtho(
            mesh->facetsIndices,
            mesh->facets,
            mesh->texCoords,
            Float4(1.0f, 1.0f, 0.0f, 0.25f),
            Float4(0.0f, 1.0f, 1.0f, 0.25f));

        return mesh;
    }

    Mesh * const Mesh::buildPlaneMesh()
    {
        Mesh * const mesh = new Mesh();

        mesh->vertices.reserve(4);
        mesh->vertices.push_back(Float4(-1.0f, -1.0f, 0.0f, 1.0f));
        mesh->vertices.push_back(Float4(-1.0f, 1.0f, 0.0f, 1.0f));
        mesh->vertices.push_back(Float4(1.0f, 1.0f, 0.0f, 1.0f));
        mesh->vertices.push_back(Float4(1.0f, -1.0f, 0.0f, 1.0f));

        mesh->facetsIndices.reserve(2);
        mesh->facetsIndices.push_back(UInt3(0, 1, 2));
        mesh->facetsIndices.push_back(UInt3(2, 3, 0));

        mesh->setupMesh();
        computeTexCoordsOrtho(
            mesh->facetsIndices,
            mesh->facets,
            mesh->texCoords,
            Float4(1.0f, 0.0f, 0.0f, 0.5f),
            Float4(0.0f, 1.0f, 0.0f, 0.5f));

        return mesh;
    }

    Mesh * const Mesh::buildTriangleMesh()
    {
        Mesh * const mesh = new Mesh();

        mesh->vertices.reserve(3);
        mesh->vertices.push_back(Float4(-0.5f, 0.0f, 0.0f, 1.0f));
        mesh->vertices.push_back(Float4(0.0f, 1.0f, 0.0f, 1.0f));
        mesh->vertices.push_back(Float4(0.5f, 0.0f, 0.0f, 1.0f));

        mesh->facetsIndices.push_back(UInt3(0, 1, 2));

        mesh->setupMesh();
        computeTexCoordsOrtho(
            mesh->facetsIndices,
            mesh->facets,
            mesh->texCoords,
            Float4(1.0f, 0.0f, 0.0f, 0.5f),
            Float4(0.0f, 1.0f, 0.0f, 0.5f));

        return mesh;
    }

    const Float4 Mesh::findFacetIntersection(const Facet & trianglePlanes, const Raycast & r)
    {
        return Float4();
    }

    Mesh * const  Mesh::loadFromOffFile(const std::string & filename, const bool flipNormals, const KDTreeTraverser<FacetIntersection> * const traverser, const KDTreeBalancer * const balancer)
    {
        Mesh * const mesh = new Mesh(traverser, balancer);
        if (filename.empty())
        {
            std::cerr << "File name is empty" << std::endl;
            return mesh;
        }

        std::ifstream file = std::ifstream(filename.c_str());
        if (!file.is_open())
        {
            std::cerr << "Failed to open " << filename << std::endl;
            return mesh;
        }

        // read all in memory and then read the content from memory
        std::stringstream fileStream;
        fileStream.str(std::string(
            std::istreambuf_iterator<char>(file),
            std::istreambuf_iterator<char>()));
        file.close();

        std::stringstream lineStream;
        std::string line, headerField;

        // read until header is found
        while (getline(fileStream, line))
        {
            lineStream.str(line);
            lineStream >> headerField;
            lineStream.clear();
            if (headerField == "OFF") break;
        }
        if (headerField != "OFF")
        {
            std::cerr << "File " << filename << " is not an OFF-file" << std::endl;
            return mesh;
        }

        // skip empty lines
        line = "";
        while (line.empty()) {
            getline(fileStream, line);
        }

        // read number of vertices and faces
        ASizeT verticesCount, facetsCount;
        lineStream.str(line);
        lineStream >> verticesCount >> facetsCount;
        lineStream.clear();

        // read vertices
        mesh->vertices.resize(verticesCount);
        for (ASizeT i = Zero<ASizeT>(); i < verticesCount; i++)
        {
            // skip empty lines
            line = "";
            while (line.empty())
            {
                getline(fileStream, line);
            }

            Float x, y, z;
            lineStream.str(line);
            lineStream >> x >> y >> z;
            lineStream.clear();
            mesh->vertices[i] = Float4(x, y, z, One<Float>());
        }

        // read faces
        mesh->facetsIndices.resize(facetsCount);
        for (ASizeT i = Zero<ASizeT>(); i < facetsCount; i++)
        {
            // skip empty lines
            line = "";
            while (line.empty())
            {
                getline(fileStream, line);
            }

            ASizeT faceVerticesCount;
            lineStream.str(line);
            lineStream >> faceVerticesCount;
            if (faceVerticesCount != 3)
            {
                std::cerr << filename << " is corrupted!" << std::endl;
                return mesh;
            }

            ASizeT v1, v2, v3;
            lineStream >> v1 >> v2 >> v3;
            lineStream.clear();
            if (flipNormals)
                mesh->facetsIndices[i] = UInt3(UInt(v1), UInt(v3), UInt(v2));
            else
                mesh->facetsIndices[i] = UInt3(UInt(v1), UInt(v2), UInt(v3));
        }

        mesh->setupMesh();
        computeTexCoordsSpherical(mesh->facetsIndices, mesh->facets, mesh->texCoords);

        std::cout << "loaded " << filename << ": " << verticesCount << " vertices, " << facetsCount << " faces" << std::endl;

        return mesh;
    }

    //}
#pragma endregion

#pragma region protected
    //{protected

    void Mesh::clear()
    {
        facetsIndices.clear();
        vertices.clear();
        vertexNormals.clear();
        facets.clear();
        texCoords.clear();
        smoothNormals.clear();
        flatNormals.clear();
        facetsEdges.clear();
    }

    void Mesh::setupMesh()
    {
        computeStandardMesh(bounding, vertices);
        computeFacets(facetsIndices, vertices, facets);
        computeNormals(facetsIndices, vertices, vertexNormals, facets, smoothNormals, flatNormals, facetsEdges);
    }

    //}
#pragma endregion

}
