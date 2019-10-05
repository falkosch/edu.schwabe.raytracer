#include "Benchmarks.h"
#include "stdafx.h"

#include <iostream>
#include <thread>
#include <array>

namespace raytracerui
{
    // Ray-AxisAlignedBoundingBox overlaps test
    ARCH_NOINLINE const bool benchmarkAABBOverlaps(
        const std::array<Raycast, 4> & r,
        const ASizeT iterations
    ) {
        auto a = AxisAlignedBoundingBox(
            Float4(-1.f, -1.f, -1.f, 1.f),
            Float4(2.f, 1.f, 1.f, 1.f)
        );
        auto b = AxisAlignedBoundingBox(
            Float4(-1.f, -1.f, 1.f, 1.f),
            Float4(1.f, 1.f, 2.f, 1.f)
        );

        auto out = Float4::VectorBoolType();
        for (ASizeT i = VectorIndices::X; i < iterations; ++i) {
            a.minimum = replaceX(a.minimum, x(-a.minimum));
            out = out | overlaps(r[0], a, b);
            out = out | overlaps(r[1], a, b);
            out = out | overlaps(r[2], a, b);
            out = out | overlaps(r[3], a, b);
        }
        return allTrue(out);
    }

    // Ray-AxisAlignedBoundingBox nearest intersection
    ARCH_NOINLINE const Float benchmarkAABB(
        const std::array<Raycast, 4> & r,
        const ASizeT iterations
    ) {
        auto a = AxisAlignedBoundingBox(
            Float4(-1.f, -1.f, -1.f, 1.f),
            Float4(1.f, 1.f, 1.f, 1.f)
        );

        auto out = Zero<Float>();
        for (ASizeT i = VectorIndices::X; i < iterations; ++i) {
            a.minimum = replaceX(a.minimum, x(-a.minimum));
            out += nearestIntersection(r[0], a, MaskAll<Size2::ValueType>());
            out += nearestIntersection(r[1], a, MaskAll<Size2::ValueType>());
            out += nearestIntersection(r[2], a, MaskAll<Size2::ValueType>());
            out += nearestIntersection(r[3], a, MaskAll<Size2::ValueType>());
        }
        return out;
    }

    // Ray-BoundingSphere nearest intersection
    ARCH_NOINLINE const Float benchmarkSphere(
        const std::array<Raycast, 4> & r,
        const ASizeT iterations
    ) {
        auto a = BoundingSphere(Float4(0.f, 0.f, 0.f, 1.f), 1.0f);

        auto out = Zero<Float>();
        for (ASizeT i = VectorIndices::X; i < iterations; ++i) {
            a.centerRadius = replaceX(a.centerRadius, x(-a.centerRadius));
            out += nearestIntersection(r[0], a, MaskAll<Size2::ValueType>());
            out += nearestIntersection(r[1], a, MaskAll<Size2::ValueType>());
            out += nearestIntersection(r[2], a, MaskAll<Size2::ValueType>());
            out += nearestIntersection(r[3], a, MaskAll<Size2::ValueType>());
        }
        return out;
    }

    // Ray-SplittingPlane nearest intersection
    ARCH_NOINLINE const Float benchmarkPlane(
        const std::array<Raycast, 4> & r,
        const ASizeT iterations
    ) {
        auto a = SplittingPlane(Float4(0.f, 0.f, 0.f, 0.f), Float4(0.f, 0.f, -1.f, 0.f));

        auto out = Zero<Float>();
        for (ASizeT i = VectorIndices::X; i < iterations; ++i) {
            a.normalDistance = replaceW(a.normalDistance, w(-a.normalDistance));
            out += nearestIntersection(r[0], a, MaskAll<Size2::ValueType>());
            out += nearestIntersection(r[1], a, MaskAll<Size2::ValueType>());
            out += nearestIntersection(r[2], a, MaskAll<Size2::ValueType>());
            out += nearestIntersection(r[3], a, MaskAll<Size2::ValueType>());
        }
        return out;
    }

    // Ray-Mesh nearest intersection
    ARCH_NOINLINE const Float benchmarkMeshFacet(
        const std::array<Raycast, 4> & r,
        const ASizeT iterations
    ) {
        auto m = Mesh::buildTriangleMesh();
        auto f1 = FacetIntersection();
        auto f2 = FacetIntersection();

        auto out = Zero<Float>();
        for (ASizeT i = VectorIndices::X; i < iterations; ++i) {
            out += m->findNearestIntersection(r[0], &f1, f2);
            out += m->findNearestIntersection(r[1], &f1, f2);
            out += m->findNearestIntersection(r[2], &f1, f2);
            out += m->findNearestIntersection(r[3], &f1, f2);
        }

        delete m;
        return out;
    }

    const Int_64 benchmarkCPUClockFrequency() {
        LARGE_INTEGER freq, time, stopTime;
        QueryPerformanceFrequency(&freq);
        QueryPerformanceCounter(&time);
        stopTime.QuadPart = time.QuadPart + (freq.QuadPart << 2);

        const Int_64 start = __rdtsc();
        for (; time.QuadPart < stopTime.QuadPart;) {
            QueryPerformanceCounter(&time);
        }
        const Int_64 stop = __rdtsc();
        const Int_64 cpuClockFrequency = (stop - start) >> 2;

        std::cout << freq.QuadPart << " " << cpuClockFrequency << std::endl;

        return cpuClockFrequency;
    }

    void runBenchmarks() {
        benchmarkCPUClockFrequency();
        const ASizeT iterations{ 100000000 };
        const Float_64 timeFactor = reciprocal(Float_64{ 4 * iterations });

        Int_64 start, stop;
        const std::array<Raycast, 4> r = {
            Raycast(
                Ray(
                    Float4(0.f, 0.f, -4.f, 1.f),
                    Float4(0.f, 0.f, 1.f, 0.f)
                ),
                cullingOrientationToMask(NegativeOne<Int>()),
                Size2(),
                100.f
            ),
            Raycast(
                Ray(
                    Float4(0.f, 0.f, 4.f, 1.f),
                    Float4(0.f, 0.f, -1.f, 0.f)
                ),
                cullingOrientationToMask(NegativeOne<Int>()),
                Size2(),
                100.f
            ),
            Raycast(
                Ray(
                    Float4(4.f, 0.f, 0.f, 1.f),
                    Float4(-1.f, 0.f, 0.f, 0.f)
                ),
                cullingOrientationToMask(NegativeOne<Int>()),
                Size2(),
                100.f
            ),
            Raycast(
                Ray(
                    Float4(0.f, 4.f, 0.f, 1.f),
                    Float4(0.f, -1.f, 0.f, 0.f)
                ),
                cullingOrientationToMask(NegativeOne<Int>()),
                Size2(),
                100.f
            )
        };

        {
            // Ray-AABBx2 overlaps benchmark (=traversal costs of kd-tree)
            start = __rdtsc();
            const bool b1 = benchmarkAABBOverlaps(r, iterations);
            stop = __rdtsc();
            const Float_64 b1t = static_cast<Float_64>(stop - start) * timeFactor;
            std::cout << "benchmarkAABBOverlaps " << b1 << " " << b1t << std::endl;
        }

        {
            // Ray-AABB nearest intersection benchmark (Box)
            start = __rdtsc();
            const Float b2 = benchmarkAABB(r, iterations);
            stop = __rdtsc();
            const Float_64 b2t = static_cast<Float_64>(stop - start) * timeFactor;
            std::cout << "benchmarkAABB " << b2 << " " << b2t << std::endl;
        }

        {
            // Ray-BoundingSphere nearest intersection benchmark (Sphere)
            start = __rdtsc();
            const Float b3 = benchmarkSphere(r, iterations);
            stop = __rdtsc();
            const Float_64 b3t = static_cast<Float_64>(stop - start) * timeFactor;
            std::cout << "benchmarkSphere " << b3 << " " << b3t << std::endl;
        }

        {
            // Ray-SplittingPlane nearest intersection benchmark (Plane)
            start = __rdtsc();
            const Float b4 = benchmarkPlane(r, iterations);
            stop = __rdtsc();
            const Float_64 b4t = static_cast<Float_64>(stop - start) * timeFactor;
            std::cout << "benchmarkPlane " << b4 << " " << b4t << std::endl;
        }

        {
            // Ray-Triangle nearest intersection benchmark (MeshFacet)
            start = __rdtsc();
            const Float b5 = benchmarkMeshFacet(r, iterations);
            stop = __rdtsc();
            const Float_64 b5t = static_cast<Float_64>(stop - start) * timeFactor;
            std::cout << "benchmarkMeshFacet " << b5 << " " << b5t << std::endl;
        }
    }

    void Benchmarks::operator()() const {
        HANDLE thisProcess = GetCurrentProcess();
        DWORD_PTR originalAffinityMask{}, t{};
        GetProcessAffinityMask(thisProcess, &originalAffinityMask, &t);
        SetProcessAffinityMask(thisProcess, 0);

        runBenchmarks();

        SetProcessAffinityMask(thisProcess, originalAffinityMask);
    }
}