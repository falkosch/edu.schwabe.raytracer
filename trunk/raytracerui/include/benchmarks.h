#pragma once

// Ray-AABBx2 overlaps test
const bool benchmarkAABBOverlaps(const primitives::Raycast & r, const vectorization::ASizeT iterations)
{
    primitives::AxisAlignedBoundingBox a = primitives::AxisAlignedBoundingBox(
        vectorization::Float4(-1.f, -1.f, -1.f, 1.f),
        vectorization::Float4(0.f, 1.f, 1.f, 1.f));
    primitives::AxisAlignedBoundingBox b = primitives::AxisAlignedBoundingBox(
        vectorization::Float4(0.f, -1.f, -1.f, 1.f),
        vectorization::Float4(1.f, 1.f, 1.f, 1.f));

    // trick the compiler to not optimize the calculations away
    volatile bool out = false;
    for (vectorization::ASizeT i = vectorization::VectorIndices::X; i < iterations; ++i)
    {
        out = out || vectorization::allTrue(primitives::overlaps(r, a, b));
        out = out || vectorization::allTrue(primitives::overlaps(r, a, b));
        out = out || vectorization::allTrue(primitives::overlaps(r, a, b));
        out = out || vectorization::allTrue(primitives::overlaps(r, a, b));
    }
    return out;
}

// Ray-AABB nearest intersection
const vectorization::Float benchmarkAABB(const primitives::Raycast & r, const vectorization::ASizeT iterations)
{
    primitives::AxisAlignedBoundingBox a = primitives::AxisAlignedBoundingBox(
        vectorization::Float4(-1.f, -1.f, -1.f, 1.f),
        vectorization::Float4(1.f, 1.f, 1.f, 1.f));

    // trick the compiler to not optimize the calculations away
    volatile vectorization::Float out = vectorization::Zero<vectorization::Float>();
    for (vectorization::ASizeT i = vectorization::VectorIndices::X; i < iterations; ++i)
    {
        out += primitives::nearestIntersection(r, a, vectorization::MaskAll<vectorization::Size2::ValueType>());
        out += primitives::nearestIntersection(r, a, vectorization::MaskAll<vectorization::Size2::ValueType>());
        out += primitives::nearestIntersection(r, a, vectorization::MaskAll<vectorization::Size2::ValueType>());
        out += primitives::nearestIntersection(r, a, vectorization::MaskAll<vectorization::Size2::ValueType>());
    }
    return out;
}

// Ray-BoundingSphere nearest intersection
const vectorization::Float benchmarkSphere(const primitives::Raycast & r, const vectorization::ASizeT iterations)
{
    primitives::BoundingSphere a = primitives::BoundingSphere(vectorization::Float4(0.1f, 0.2f, 0.3f, 1.f), 2.0f);

    // trick the compiler to not optimize the calculations away
    volatile vectorization::Float out = vectorization::Zero<vectorization::Float>();
    for (vectorization::ASizeT i = vectorization::VectorIndices::X; i < iterations; ++i)
    {
        out += primitives::nearestIntersection(r, a, vectorization::MaskAll<vectorization::Size2::ValueType>());
        a.centerRadius.setX(-x(a.centerRadius));
        out += primitives::nearestIntersection(r, a, vectorization::MaskAll<vectorization::Size2::ValueType>());
        a.centerRadius.setY(-y(a.centerRadius));
        out += primitives::nearestIntersection(r, a, vectorization::MaskAll<vectorization::Size2::ValueType>());
        a.centerRadius.setZ(-z(a.centerRadius));
        out += primitives::nearestIntersection(r, a, vectorization::MaskAll<vectorization::Size2::ValueType>());
        a.centerRadius.setX(-x(a.centerRadius));
    }
    return out;
}

// Ray-SplittingPlane nearest intersection
const vectorization::Float benchmarkPlane(const primitives::Raycast & r, const vectorization::ASizeT iterations)
{
    primitives::SplittingPlane a = primitives::SplittingPlane();

    // trick the compiler to not optimize the calculations away
    volatile vectorization::Float out = vectorization::Zero<vectorization::Float>();
    for (vectorization::ASizeT i = vectorization::VectorIndices::X; i < iterations; ++i)
    {
        out += primitives::nearestIntersection(r, a, vectorization::MaskAll<vectorization::Size2::ValueType>());
        out += primitives::nearestIntersection(r, a, vectorization::MaskAll<vectorization::Size2::ValueType>());
        out += primitives::nearestIntersection(r, a, vectorization::MaskAll<vectorization::Size2::ValueType>());
        out += primitives::nearestIntersection(r, a, vectorization::MaskAll<vectorization::Size2::ValueType>());
    }
    return out;
}

// Ray-SplittingPlane nearest intersection
const vectorization::Float benchmarkMeshFacet(const primitives::Raycast & r, const vectorization::ASizeT iterations)
{
    Float4 v0 = Float4(-4.f, 4.f, -1.f, 1.f);
    Float4 v1 = Float4(4.f, 4.f, -1.f, 1.f);
    Float4 v2 = Float4(0.f, -4.f, -1.f, 1.f);
    Facet facet = Facet(v0, v1, v2);
    Float4 edge0 = facet.v1 - facet.v0, edge2 = facet.v2 - facet.v0;
    Float4 planeNormal0 = cross3(edge0, edge2);
    Float4 planeNormal1 = cross3(edge2, planeNormal0);
    Float4 planeNormal2 = cross3(planeNormal0, edge0);
    MeshGeometryNode facetInfo = MeshGeometryNode(0, facet);
    facetInfo.trianglePlanes.v0 = replaceW(planeNormal0, -dot3(facet.v0, planeNormal0));
    facetInfo.trianglePlanes.v1 = replaceW(planeNormal1, -dot3(facet.v0, planeNormal1));
    facetInfo.trianglePlanes.v2 = replaceW(planeNormal2, -dot3(facet.v0, planeNormal2));

    // trick the compiler to not optimize the calculations away
    volatile vectorization::Float out = vectorization::Zero<vectorization::Float>();
    for (vectorization::ASizeT i = vectorization::VectorIndices::X; i < iterations; ++i)
    {
        out += x(Mesh::findFacetIntersection(facetInfo.trianglePlanes, r));
        out += y(Mesh::findFacetIntersection(facetInfo.trianglePlanes, r));
        out += z(Mesh::findFacetIntersection(facetInfo.trianglePlanes, r));
        out += w(Mesh::findFacetIntersection(facetInfo.trianglePlanes, r));
    }
    return out;
}

void benchmarks()
{
    Raycast r = Raycast(
        Ray(Float4(.5f, 0.f, -2.f, 1.f), OneW<Float4>()),
        cullingOrientationToMask(NegOne<Int>()),
        Zero<Size2>(),
        100.f);

    LARGE_INTEGER freq, start, stop;
    QueryPerformanceFrequency(&freq);
    const ASizeT iterations = 10000000;
    const Float_64 timeFactor = 4.0 * static_cast<Float_64>(iterations)* static_cast<Float_64>(freq.QuadPart);

    // Ray-AABBx2 overlaps benchmark (=traversal costs of kd-tree)
    QueryPerformanceCounter(&start);
    const bool b1 = benchmarkAABBOverlaps(r, iterations);
    QueryPerformanceCounter(&stop);
    const Float_64 b1t = static_cast<Float_64>(stop.QuadPart - start.QuadPart) / timeFactor;

    // Ray-AABB nearest intersection benchmark (Box)
    QueryPerformanceCounter(&start);
    const Float b2 = benchmarkAABB(r, iterations);
    QueryPerformanceCounter(&stop);
    const Float_64 b2t = static_cast<Float_64>(stop.QuadPart - start.QuadPart) / timeFactor;

    // Ray-BoundingSphere nearest intersection benchmark (Sphere)
    QueryPerformanceCounter(&start);
    const Float b3 = benchmarkSphere(r, iterations);
    QueryPerformanceCounter(&stop);
    const Float_64 b3t = static_cast<Float_64>(stop.QuadPart - start.QuadPart) / timeFactor;

    // Ray-SplittingPlane nearest intersection benchmark (Plane)
    QueryPerformanceCounter(&start);
    const Float b4 = benchmarkPlane(r, iterations);
    QueryPerformanceCounter(&stop);
    const Float_64 b4t = static_cast<Float_64>(stop.QuadPart - start.QuadPart) / timeFactor;

    // Ray-Triangle nearest intersection benchmark (MeshFacet)
    QueryPerformanceCounter(&start);
    const Float b5 = benchmarkMeshFacet(r, iterations);
    QueryPerformanceCounter(&stop);
    const Float_64 b5t = static_cast<Float_64>(stop.QuadPart - start.QuadPart) / timeFactor;

    std::cout << "Ray-AABBx2 overlaps" << b1 << " " << b1t << std::endl;
    std::cout << "Ray-AABB nearest" << b2 << " " << b2t << std::endl;
    std::cout << "Ray-Sphere nearest" << b3 << " " << b3t << std::endl;
    std::cout << "Ray-Plane nearest" << b4 << " " << b4t << std::endl;
    std::cout << "Ray-Triangle nearest" << b5 << " " << b5t << std::endl;
}
