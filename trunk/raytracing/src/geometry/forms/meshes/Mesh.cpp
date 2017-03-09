#include "../../../stdafx.h"

#include "geometry/forms/meshes/Mesh.h"

#include <sstream>
#include <fstream>

namespace raytracer
{

    Mesh::Mesh()
        :
        vertices(),
        facetsIndices(),
        vertexNormals(),
        facets(),
        texCoords(),
        smoothNormals(),
        flatNormals(),
        facetsEdges()
    {
    }

    Mesh::Mesh(const std::string file, const bool hasRevertedNormals, const KDTreeTraverser<FacetIntersection> * const traverser, const KDTreeBalancer * const balancer)
    {
        loadOff(file, hasRevertedNormals);
    }

    Mesh::~Mesh()
    {
        clear();
    }

    void Mesh::clear()
    {
        vertices.clear();
        facetsIndices.clear();
        vertexNormals.clear();
        facets.clear();
        facetsEdges.clear();
        texCoords.clear();
        smoothNormals.clear();
        flatNormals.clear();
    }

    void Mesh::buildTriangleMesh()
    {
        clear();

        vertices.reserve(3);
        vertices.push_back(Float4(-0.5f, 0.0f, 0.0f, 1.0f));
        vertices.push_back(Float4(0.0f, 1.0f, 0.0f, 1.0f));
        vertices.push_back(Float4(0.5f, 0.0f, 0.0f, 1.0f));

        facetsIndices.push_back(UInt3(0, 1, 2));

        setupMesh();
        calculateTexCoordsOrtho(
            Float4(1.0f, 0.0f, 0.0f, 0.5f),
            Float4(0.0f, 1.0f, 0.0f, 0.5f));
    }

    void Mesh::buildPlaneMesh()
    {
        clear();

        vertices.reserve(4);
        vertices.push_back(Float4(-1.0f, -1.0f, 0.0f, 1.0f));
        vertices.push_back(Float4(-1.0f, 1.0f, 0.0f, 1.0f));
        vertices.push_back(Float4(1.0f, 1.0f, 0.0f, 1.0f));
        vertices.push_back(Float4(1.0f, -1.0f, 0.0f, 1.0f));

        facetsIndices.reserve(2);
        facetsIndices.push_back(UInt3(0, 1, 2));
        facetsIndices.push_back(UInt3(2, 3, 0));

        setupMesh();
        calculateTexCoordsOrtho(
            Float4(1.0f, 0.0f, 0.0f, 0.5f),
            Float4(0.0f, 1.0f, 0.0f, 0.5f));
    }

    void Mesh::buildCubeMesh()
    {
        clear();

        vertices.reserve(8);
        vertices.push_back(Float4(-1.0f, -1.0f, -1.0f, 1.0f));
        vertices.push_back(Float4(-1.0f, -1.0f, 1.0f, 1.0f));
        vertices.push_back(Float4(1.0f, -1.0f, 1.0f, 1.0f));
        vertices.push_back(Float4(1.0f, -1.0f, -1.0f, 1.0f));
        vertices.push_back(Float4(-1.0f, 1.0f, -1.0f, 1.0f));
        vertices.push_back(Float4(-1.0f, 1.0f, 1.0f, 1.0f));
        vertices.push_back(Float4(1.0f, 1.0f, 1.0f, 1.0f));
        vertices.push_back(Float4(1.0f, 1.0f, -1.0f, 1.0f));

        facetsIndices.reserve(12);
        // 2 x bottom facets
        facetsIndices.push_back(UInt3(0, 2, 1));
        facetsIndices.push_back(UInt3(2, 0, 3));
        // 2 x top facets
        facetsIndices.push_back(UInt3(4, 5, 6));
        facetsIndices.push_back(UInt3(6, 7, 4));
        // 2 x left facets
        facetsIndices.push_back(UInt3(0, 1, 5));
        facetsIndices.push_back(UInt3(5, 4, 0));
        // 2 x front facets
        facetsIndices.push_back(UInt3(1, 2, 6));
        facetsIndices.push_back(UInt3(6, 5, 1));
        // 2 x right facets
        facetsIndices.push_back(UInt3(2, 3, 7));
        facetsIndices.push_back(UInt3(7, 6, 2));
        // 2 x back facets
        facetsIndices.push_back(UInt3(3, 0, 4));
        facetsIndices.push_back(UInt3(4, 7, 3));

        setupMesh();
        calculateTexCoordsOrtho(
            Float4(1.0f, 1.0f, 0.0f, 0.25f),
            Float4(0.0f, 1.0f, 1.0f, 0.25f));
    }

    void Mesh::setupMesh()
    {
        scaleMeshIntoCanonicalCube();
        buildFacets();
        calculateNormals();
    }

    void Mesh::loadOff(const std::string & filename, const bool flipNormals)
    {
        if (filename.empty())
            return;

        clear();

        std::ifstream file(filename.c_str());

        if (!file.is_open())
        {
            std::cerr << "Failed to open " << filename << std::endl;
            return;
        }

        // read all in memory
        std::istreambuf_iterator<char> fileBegin(file);
        std::istreambuf_iterator<char> fileEnd = std::istreambuf_iterator<char>();
        std::string fileContent(fileBegin, fileEnd);
        file.close();
        std::stringstream fileStream;
        fileStream.str(fileContent);

        std::stringstream lineStream;
        std::string line, headerField;

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
            return;
        }

        // skip empty lines
        line = "";
        while (line.empty())
            getline(fileStream, line);

        // read number of vertices and faces
        ASizeT verticesCount, facetsCount;
        lineStream.str(line);
        lineStream >> verticesCount >> facetsCount;
        lineStream.clear();

        // read vertices
        vertices.resize(verticesCount);
        for (ASizeT i = 0; i < verticesCount; i++)
        {
            // skip empty lines
            line = "";
            while (line.empty())
                getline(fileStream, line);

            Float x, y, z;
            lineStream.str(line);
            lineStream >> x >> y >> z;
            lineStream.clear();
            vertices[i] = Float4(x, y, z, 1.0f);
        }

        // read faces
        facetsIndices.resize(facetsCount);
        for (ASizeT i = 0; i < facetsCount; i++)
        {
            // skip empty lines
            line = "";
            while (line.empty())
                getline(fileStream, line);

            ASizeT faceVerticesCount;
            lineStream.str(line);
            lineStream >> faceVerticesCount;
            if (faceVerticesCount != 3)
            {
                std::cerr << filename << " is corrupted!" << std::endl;
                return;
            }

            ASizeT v1, v2, v3;
            lineStream >> v1 >> v2 >> v3;
            lineStream.clear();
            if (flipNormals)
                facetsIndices[i] = UInt3(v1, v3, v2);
            else
                facetsIndices[i] = UInt3(v1, v2, v3);
        }

        setupMesh();
        calculateTexCoordsSpherical();

        std::cout << "loaded " << filename << ": " << verticesCount << " vertices, " << facetsCount << " faces" << std::endl;
        return;
    }

    void Mesh::scaleMeshIntoCanonicalCube()
    {
        const ASizeT verticesCount = vertices.size();
        const int verticesCounti = (int)verticesCount;
        Float4 scalev,
            meshMin(FLT_MAX, FLT_MAX, FLT_MAX, 0.0f),
            meshMax(FLT_MIN, FLT_MIN, FLT_MIN, 0.0f);

        // get extends
        for (int i = 0; i < verticesCounti; i++)
        {
            const Float4* v = &vertices[i];
            meshMin = min(*v, meshMin);
            meshMax = max(*v, meshMax);
        }

        // calculate scale (which is the largest axis-aligned extend)
        Float4 center = meshMax - meshMin; // compute the axis-aligned extend of the mesh
        Float scale = min3(center);

        // shouldn't happen, otherwise all vertices lay on one point
        if (scale != 0.0f) {
            scale = 1.0f / scale;
        }
        scalev = Float4(scale, scale, scale, 1.0f);

        // calculate center
        center = (meshMax + meshMin) * 0.5f;

        // scale and translate
#pragma omp parallel for
        for (int i = 0; i < verticesCounti; i++)
        {
            vertices[i] = replaceW((vertices[i] - center) * scalev, 1.0f);
        }
    }

    void Mesh::buildFacets()
    {
        const ASizeT facetsCount = facetsIndices.size();
        const int facetsCounti = (int)facetsCount;
        facets.resize(facetsCount);

#pragma omp parallel for
        for (int i = 0; i < facetsCounti; i++)
        {
            const UInt3* facetIndices = &facetsIndices[i];
            Facet* facet = &facets[i];
            facet->v0 = vertices[x(*facetIndices)];
            facet->v1 = vertices[y(*facetIndices)];
            facet->v2 = vertices[z(*facetIndices)];
        }
    }

    void Mesh::calculateNormals()
    {
        const ASizeT verticesCount = vertices.size(), facetsCount = facetsIndices.size();
        const int facetsCounti = (int)facetsCount;
        const int verticesCounti = (int)verticesCount;
        UInt3 *facetIndices;
        Facet *normals;

        facetsEdges.resize(facetsCount);
        vertexNormals.resize(verticesCount);
        flatNormals.resize(facetsCount);
        smoothNormals.resize(facetsCount);

#pragma omp parallel for private(normals,facetIndices)
        for (int i = 0; i < facetsCounti; i++)
        {
            const Facet *facet = &facets[i];

            // edges in counter-clockwise order
            Float4 edge0 = zeroW(facet->v0 - facet->v1);
            Float4 edge1 = zeroW(facet->v1 - facet->v2);
            Float4 edge2 = zeroW(facet->v2 - facet->v0);
            const Float l0 = length3(edge0);
            const Float l1 = length3(edge1);
            const Float l2 = length3(edge2);

            // precomputes for raytracing
            FacetEdges *facetEdges = &facetsEdges[i];
            facetEdges->v0 = -edge0;
            facetEdges->v1 = edge2;

            // set face normal for each face vertex
            normals = &flatNormals[i];
            normals->v0 = normals->v1 = normals->v2 = normalize3(cross3(edge0, edge1));

            // calculate weighted face normal part for the vertex normals
            facetIndices = &facetsIndices[i];
            Facet normalsWeighted;
            normalsWeighted.v0 = normals->v0 / (l0 * l2);
            normalsWeighted.v1 = normals->v0 / (l1 * l0);
            normalsWeighted.v2 = normals->v0 / (l2 * l1);

#pragma omp critical
            {
                vertexNormals[x(*facetIndices)] += normalsWeighted.v0;
                vertexNormals[y(*facetIndices)] += normalsWeighted.v1;
                vertexNormals[z(*facetIndices)] += normalsWeighted.v2;
            }
        }

        // normalize vertex-normals
#pragma omp parallel for
        for (int i = 0; i < verticesCounti; i++)
        {
            vertexNormals[i] = normalize3(vertexNormals[i]);
        }

        // build Facet-packs of the vertex-normals
#pragma omp parallel for private(facetIndices,normals)
        for (int i = 0; i < facetsCounti; i++)
        {
            // flatNormal comparison for normal adjustment at hard edges
            const Float4 flatNormal = flatNormals[i].v0;
            facetIndices = &facetsIndices[i];
            normals = &smoothNormals[i];

            normals->v0 = vertexNormals[x(*facetIndices)];
            normals->v1 = vertexNormals[y(*facetIndices)];
            normals->v2 = vertexNormals[z(*facetIndices)];

            //if (dot(normals->v0, flatNormal) <= glm::one_over_root_two<Float>())
            //    normals->v0 = flatNormal;
            //if (dot(normals->v1, flatNormal) <= glm::one_over_root_two<Float>())
            //    normals->v1 = flatNormal;
            //if (dot(normals->v2, flatNormal) <= glm::one_over_root_two<Float>())
            //    normals->v2 = flatNormal;
        }
    }

    void Mesh::calculateTexCoordsOrtho(const Float4& sPlane, const Float4& tPlane)
    {
        const ASizeT facetsCount = facetsIndices.size();
        const int facetsCounti = (int)facetsCount;
        texCoords.resize(facetsCount);

#pragma omp parallel for
        for (int i = 0; i < facetsCounti; i++)
        {
            const Facet *facet = &facets[i];
            Facet *facetTexCoords = &texCoords[i];
            facetTexCoords->v0 = Float4(dot(facet->v0, sPlane), dot(facet->v0, tPlane));
            facetTexCoords->v1 = Float4(dot(facet->v1, sPlane), dot(facet->v1, tPlane));
            facetTexCoords->v2 = Float4(dot(facet->v2, sPlane), dot(facet->v2, tPlane));
        }
    }

    void Mesh::calculateTexCoordsSpherical()
    {
        const ASizeT facetsCount = facetsIndices.size();
        const int facetsCounti = (int)facetsCount;

        // calculate tex-coords for each vertex-normal
        texCoords.resize(facetsCount);
#pragma omp parallel for
        for (int i = 0; i < facetsCounti; i++)
        {
            Facet facet = facets[i];

            // normalize vertices, they are used as normals for determining the texcoords
            // "- Common::ONE_W" elimates w-coord, which is always 1.0f for vertices
            facet.v0 = normalize3(facet.v0);
            facet.v1 = normalize3(facet.v1);
            facet.v2 = normalize3(facet.v2);

            // calculate latitude coords
            Facet *facetTexCoords = &texCoords[i];


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

    // intersection test based on http://www.graphics.cornell.edu/pubs/1997/MT97.pdf
    const bool Mesh::intersectsFacet(const ASizeT facetIndex, const Ray& ray, const Float maxDistance)
    {
        if (maxDistance <= 0.0f)
            return false;

        const Facet *facet = &facets[facetIndex];
        const FacetEdges *facetEdges = &facetsEdges[facetIndex];

        // calculate determinant (also part of the barycentric-U-coord)
        const Float4 pvec = cross3(ray.direction, facetEdges->v1);
        const Float det = 1.0f / dot(facetEdges->v0, pvec);

        // calculate distance from v0 to ray.origin
        const Float4 tvec = ray.origin - facet->v0;

        // triangle bound check on U
        const Float u = dot(tvec, pvec) * det;
        if (u < 0.0f || u > 1.0f)
            return false;

        // triangle bound check on V
        const Float4 qvec = cross3(tvec, facetEdges->v0);
        const Float v = dot(ray.direction, qvec) * det;
        if (v < 0.0f || u + v > 1.0f)
            return false;

        // calculate t (in "orig + t * dir" == "ray.origin + t * ray.direction")
        const Float t = dot(facetEdges->v1, qvec) * det;

        // depth-test
        return t >= 0.0f && t < maxDistance;
    }

    const Float Mesh::findFacetIntersection(const ASizeT facetIndex, const Ray& ray, const Float maxDistance, FacetIntersection* intersectionOut)
    {
        const Facet *facet = &facets[facetIndex];
        const FacetEdges *facetEdges = &facetsEdges[facetIndex];
        // calculate determinant (also part of the barycentric-U-coord)
        const Float4 pvec = cross3(ray.direction, facetEdges->v1);
        const Float det = 1.0f / dot(facetEdges->v0, pvec);

        // calculate distance from v0 to ray.origin
        const Float4 tvec = ray.origin - facet->v0;

        // triangle bound check on U
        const Float u = dot(tvec, pvec) * det;
        if (u < 0.0f || u > 1.0f)
            return maxDistance;

        // triangle bound check on V
        const Float4 qvec = cross3(tvec, facetEdges->v0);
        const Float v = dot(ray.direction, qvec) * det;
        if (v < 0.0f || u + v > 1.0f)
            return maxDistance;

        // calculate t (in "orig + t * dir" == "ray.origin + t * ray.direction")
        const Float t = dot(facetEdges->v1, qvec) * det;

        // depth-test
        if (t < 0.0f || t >= maxDistance) {
            return maxDistance;
        }

        if (intersectionOut) {
            intersectionOut->texCoords = Float4(u, v);
        }
        return t;
    }

    const bool Mesh::intersects(const Ray& ray, const Float maxDistance, const ASizeT excludeId)
    {
        if (maxDistance <= 0.0f) {
            return false;
        }

        const ASizeT facetsCount = facets.size();
        for (ASizeT i = 0; i < facetsCount; i++)
        {
            // intersection test based on http://www.graphics.cornell.edu/pubs/1997/MT97.pdf
            const Facet *facet = &facets[i];
            if (excludeId == (ASizeT)facet) {
                continue;
            }

            if (intersectsFacet(i, ray, maxDistance)) {
                return true;
            }
        }
        return false;
    }

    const Float Mesh::findNearestIntersection(const Ray& ray, const Float maxDistance, const ASizeT excludeId, FacetIntersection* intersectionOut)
    {
        if (maxDistance <= 0.0f) {
            return maxDistance;
        }

        ASizeT intersected;
        Float distance = maxDistance;
        Float4 preservedUV;
        const ASizeT facetsCount = facets.size();
        for (ASizeT i = 0; i < facetsCount; i++)
        {
            // intersection test based on http://www.graphics.cornell.edu/pubs/1997/MT97.pdf
            const Facet *facet = &facets[i];
            if (excludeId == (ASizeT)facet) {
                continue;
            }

            const Float t = findFacetIntersection(i, ray, maxDistance, intersectionOut);
            if (t < distance)
            {
                distance = t;
                intersected = i;
                // we have to save our uv-vector separately,
                // otherwise findFacetIntersection will always overwrite it on any intersection
                if (intersectionOut) {
                    preservedUV = intersectionOut->texCoords;
                }

            }
        }

        // was there an intersection?
        if (distance >= maxDistance) {
            return maxDistance;
        }

        if (intersectionOut)
        {
            const Facet* vNormals = &smoothNormals[intersected];
            const Facet* vTexCoords = &texCoords[intersected];

            // use triangle-point equation: T(u, v) = (1 - u - v) * V0 + u * V1 + v * V2
            const Float4 barycenterNormal = vNormals->v0
                + xxxx(preservedUV) * (vNormals->v1 - vNormals->v0)
                + yyyy(preservedUV) * (vNormals->v2 - vNormals->v0);
            const Float4 barycenterTex = vTexCoords->v0
                + xxxx(preservedUV) * (vTexCoords->v1 - vTexCoords->v0)
                + yyyy(preservedUV) * (vTexCoords->v2 - vTexCoords->v0);

            //intersectionOut->id = (ASizeT)& facets[intersected];
            intersectionOut->vertex = ray.origin + distance * ray.direction;
            intersectionOut->smoothedNormal = normalize3(barycenterNormal);
            intersectionOut->msSurfaceNormal = flatNormals[intersected].v0;
            intersectionOut->surfaceNormal = flatNormals[intersected].v0;
            intersectionOut->texCoords = xy_zw(barycenterTex, OneW<Float4>());
        }
        return distance;
    }

}
