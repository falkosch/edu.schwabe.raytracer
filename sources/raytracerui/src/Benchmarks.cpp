#include "Benchmarks.h"
#include "stdafx.h"

#include <array>
#include <iostream>

namespace raytracerui {
  // Ray-AxisAlignedBoundingBox overlaps test
  ARCH_NOINLINE bool benchmarkAABBOverlaps(const std::array<RayCast, 4> &rayCasts, const ASizeT iterations) {
    auto boundingBox1 = AxisAlignedBoundingBox(Float4(-1.f, -1.f, -1.f, 1.f), Float4(2.f, 1.f, 1.f, 1.f));
    auto boundingBox2 = AxisAlignedBoundingBox(Float4(-1.f, -1.f, 1.f, 1.f), Float4(1.f, 1.f, 2.f, 1.f));

    auto dummy = Float4::VectorBoolType();
    for (auto i{VectorIndices::X}; i < iterations; ++i) {
      boundingBox1.minimum = replaceX(boundingBox1.minimum, x(-boundingBox1.minimum));
      dummy = dummy | overlaps(rayCasts[0], boundingBox1, boundingBox2);
      dummy = dummy | overlaps(rayCasts[1], boundingBox1, boundingBox2);
      dummy = dummy | overlaps(rayCasts[2], boundingBox1, boundingBox2);
      dummy = dummy | overlaps(rayCasts[3], boundingBox1, boundingBox2);
    }
    return allTrue(dummy);
  }

  // Ray-AxisAlignedBoundingBox nearest intersection
  ARCH_NOINLINE Float benchmarkAABB(const std::array<RayCast, 4> &rayCasts, const ASizeT iterations) {
    auto boundingBox = AxisAlignedBoundingBox(Float4(-1.f, -1.f, -1.f, 1.f), Float4(2.f, 1.f, 1.f, 1.f));

    auto dummy = Zero<Float>();
    for (auto i{VectorIndices::X}; i < iterations; ++i) {
      boundingBox.minimum = replaceX(boundingBox.minimum, x(-boundingBox.minimum));
      dummy += nearestIntersection(rayCasts[0], boundingBox, MaskAll<Size2::ValueType>());
      dummy += nearestIntersection(rayCasts[1], boundingBox, MaskAll<Size2::ValueType>());
      dummy += nearestIntersection(rayCasts[2], boundingBox, MaskAll<Size2::ValueType>());
      dummy += nearestIntersection(rayCasts[3], boundingBox, MaskAll<Size2::ValueType>());
    }
    return dummy;
  }

  // Ray-BoundingSphere nearest intersection
  ARCH_NOINLINE Float benchmarkSphere(const std::array<RayCast, 4> &rayCasts, const ASizeT iterations) {
    auto boundingSphere = BoundingSphere(Float4(0.f, 0.f, 0.f, 1.f), 0.5f);

    auto dummy = Zero<Float>();
    for (auto i{VectorIndices::X}; i < iterations; ++i) {
      boundingSphere.centerRadius = replaceX(boundingSphere.centerRadius, x(-boundingSphere.centerRadius));
      dummy += nearestIntersection(rayCasts[0], boundingSphere, MaskAll<Size2::ValueType>());
      dummy += nearestIntersection(rayCasts[1], boundingSphere, MaskAll<Size2::ValueType>());
      dummy += nearestIntersection(rayCasts[2], boundingSphere, MaskAll<Size2::ValueType>());
      dummy += nearestIntersection(rayCasts[3], boundingSphere, MaskAll<Size2::ValueType>());
    }
    return dummy;
  }

  // Ray-SplittingPlane nearest intersection
  ARCH_NOINLINE Float benchmarkPlane(const std::array<RayCast, 4> &rayCasts, const ASizeT iterations) {
    auto splittingPlane = SplittingPlane(Float4(-0.1f, -0.1f, -0.1f, 1.0f), normalize3(Float4(1.f, 1.f, 1.f, 0.f)));

    auto dummy = Zero<Float>();
    for (auto i{VectorIndices::X}; i < iterations; ++i) {
      splittingPlane.normalDistance = replaceW(splittingPlane.normalDistance, w(-splittingPlane.normalDistance));
      dummy += nearestIntersection(rayCasts[0], splittingPlane, MaskAll<Size2::ValueType>());
      dummy += nearestIntersection(rayCasts[1], splittingPlane, MaskAll<Size2::ValueType>());
      dummy += nearestIntersection(rayCasts[2], splittingPlane, MaskAll<Size2::ValueType>());
      dummy += nearestIntersection(rayCasts[3], splittingPlane, MaskAll<Size2::ValueType>());
    }
    return dummy;
  }

  // Ray-Mesh nearest intersection
  ARCH_NOINLINE Float benchmarkMeshFacet(const std::array<RayCast, 4> &rayCasts, const ASizeT iterations) {
    auto mesh = Mesh::buildTriangleMesh();
    auto f1 = FacetIntersection();
    auto f2 = FacetIntersection();

    auto dummy = Zero<Float>();
    for (auto i{VectorIndices::X}; i < iterations; ++i) {
      dummy += mesh->findNearestIntersection(rayCasts[0], &f1, f2);
      dummy += mesh->findNearestIntersection(rayCasts[1], &f1, f2);
      dummy += mesh->findNearestIntersection(rayCasts[2], &f1, f2);
      dummy += mesh->findNearestIntersection(rayCasts[3], &f1, f2);
    }

    delete mesh;
    return dummy;
  }

  UInt_64 benchmarkCPUClockFrequency() {
    LARGE_INTEGER frequency, startTime, stopTime;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&startTime);
    stopTime.QuadPart = startTime.QuadPart + (frequency.QuadPart << 2);

    auto start = __rdtsc();
    for (; startTime.QuadPart < stopTime.QuadPart;) {
      QueryPerformanceCounter(&startTime);
    }
    auto stop = __rdtsc();

    auto cpuClockFrequency = (stop - start) >> 2;
    std::cout << frequency.QuadPart << " " << cpuClockFrequency << std::endl;
    return cpuClockFrequency;
  }

  void runBenchmarks() {
    benchmarkCPUClockFrequency();
    const ASizeT iterations{100000000};
    auto timeFactor = reciprocal(Float_64{4 * iterations});

    const std::array<RayCast, 4> rayCasts = {
        RayCast(
            Ray(Float4(0.f, 0.f, -2.f, 1.f), Float4(0.f, 0.f, 1.f, 0.f)), cullingOrientationToMask(-1), Zero<Size2>(),
            5.f
        ),
        RayCast(
            Ray(Float4(0.f, 0.f, 2.f, 1.f), Float4(0.f, 0.f, -1.f, 0.f)), cullingOrientationToMask(-1), Zero<Size2>(),
            5.f
        ),
        RayCast(
            Ray(Float4(3.f, 0.f, 0.f, 1.f), Float4(-1.f, 0.f, 0.f, 0.f)), cullingOrientationToMask(-1), Zero<Size2>(),
            5.f
        ),
        RayCast(
            Ray(Float4(0.f, 2.f, 0.f, 1.f), Float4(0.f, -1.f, 0.f, 0.f)), cullingOrientationToMask(-1), Zero<Size2>(),
            5.f
        )
    };

    auto benchmark = [&rayCasts, &timeFactor](auto benchmarkScope, auto name) {
      auto start = __rdtsc();
      auto dummy = benchmarkScope(rayCasts, iterations);
      auto cycles = static_cast<Float_64>(__rdtsc() - start) * timeFactor;
      std::cout << name << ": ret=" << dummy << " cycles=" << cycles << std::endl;
    };

    benchmark(benchmarkAABBOverlaps, "Ray-AABBx2 overlaps (traversal costs of kd-tree)");
    benchmark(benchmarkAABB, "Ray-AABB nearest intersection");
    benchmark(benchmarkSphere, "Ray-BoundingSphere nearest intersection");
    benchmark(benchmarkPlane, "Ray-SplittingPlane nearest intersection");
    benchmark(benchmarkMeshFacet, "Ray-Triangle (Mesh Facet) nearest intersection");
  }

  void Benchmarks::operator()() const {
    auto thisProcess = GetCurrentProcess();
    DWORD_PTR originalAffinityMask{}, t{};
    GetProcessAffinityMask(thisProcess, &originalAffinityMask, &t);
    SetProcessAffinityMask(thisProcess, 0);

    runBenchmarks();

    SetProcessAffinityMask(thisProcess, originalAffinityMask);
  }
}