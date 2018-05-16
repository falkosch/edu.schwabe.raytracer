#include "stdafx.h"

#include "Benchmarks.h"

#include <geometry/forms/meshes/Mesh.h>

#include <thread>

namespace raytracer
{

	// Ray-AABBx2 overlaps test
	const bool benchmarkAABBOverlaps(const primitives::Raycast & r, const vectorization::ASizeT iterations)
	{
		const primitives::AxisAlignedBoundingBox a = primitives::AxisAlignedBoundingBox(
			vectorization::Float4(-1.f, -1.f, -1.f, 1.f),
			vectorization::Float4(1.f, 1.f, 1.f, 1.f)
		);
		const primitives::AxisAlignedBoundingBox b = primitives::AxisAlignedBoundingBox(
			vectorization::Float4(-2.f, -2.f, -1.f, 1.f),
			vectorization::Float4(2.f, 2.f, 2.f, 1.f)
		);

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
		const primitives::AxisAlignedBoundingBox a = primitives::AxisAlignedBoundingBox(
			vectorization::Float4(-1.f, -1.f, -1.f, 1.f),
			vectorization::Float4(1.f, 1.f, 1.f, 1.f)
		);

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
		primitives::BoundingSphere a = primitives::BoundingSphere(vectorization::Float4(0.0f, 0.0f, 0.0f, 1.f), 1.0f);

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
	const vectorization::Float benchmarkPlane(const primitives::Raycast & r, const vectorization::ASizeT iterations)
	{
		const primitives::SplittingPlane a = primitives::SplittingPlane(Float4(0.f, 0.f, 0.f, 0.f), Float4(0.f, 0.f, -1.f, 0.f));

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
		const Mesh * m = Mesh::buildTriangleMesh();
		const FacetIntersection f1;
		FacetIntersection f2;

		// trick the compiler to not optimize the calculations away
		volatile vectorization::Float out = vectorization::Zero<vectorization::Float>();
		for (vectorization::ASizeT i = vectorization::VectorIndices::X; i < iterations; ++i)
		{
			out += m->findNearestIntersection(r, &f1, f2);
			out += m->findNearestIntersection(r, &f1, f2);
			out += m->findNearestIntersection(r, &f1, f2);
			out += m->findNearestIntersection(r, &f1, f2);
		}

		delete m;
		return out;
	}

	const ASizeT benchmarkCPUClockFrequency()
	{
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

	void runBenchmarks()
	{
		const Int_64 cpuClockFrequency = benchmarkCPUClockFrequency();
		const ASizeT iterations = 4 * 100000000;
		const Float_64 timeFactor = reciprocal(static_cast<Float_64>(iterations));

		Int_64 start, stop;

		const Raycast r = Raycast(
			Ray(Float4(0.f, 0.f, -2.f, 1.f), Float4(0.f, 0.f, 1.f, 1.f)),
			cullingOrientationToMask(NegOne<Int>()),
			Zero<Size2>(),
			100.f
		);

		// Ray-AABBx2 overlaps benchmark (=traversal costs of kd-tree)
		start = __rdtsc();
		const bool b1 = benchmarkAABBOverlaps(r, iterations);
		stop = __rdtsc();
		const Float_64 b1t = static_cast<Float_64>(stop - start) * timeFactor;

		// Ray-AABB nearest intersection benchmark (Box)
		start = __rdtsc();
		const Float b2 = benchmarkAABB(r, iterations);
		stop = __rdtsc();
		const Float_64 b2t = static_cast<Float_64>(stop - start) * timeFactor;

		// Ray-BoundingSphere nearest intersection benchmark (Sphere)
		start = __rdtsc();
		const Float b3 = benchmarkSphere(r, iterations);
		stop = __rdtsc();
		const Float_64 b3t = static_cast<Float_64>(stop - start) * timeFactor;

		// Ray-SplittingPlane nearest intersection benchmark (Plane)
		start = __rdtsc();
		const Float b4 = benchmarkPlane(r, iterations);
		stop = __rdtsc();
		const Float_64 b4t = static_cast<Float_64>(stop - start) * timeFactor;

		// Ray-Triangle nearest intersection benchmark (MeshFacet)
		start = __rdtsc();
		const Float b5 = benchmarkMeshFacet(r, iterations);
		stop = __rdtsc();
		const Float_64 b5t = static_cast<Float_64>(stop - start) * timeFactor;


		std::cout << "Ray-AABBx2 overlaps " << b1 << " " << b1t << std::endl;
		std::cout << "Ray-AABB nearest " << b2 << " " << b2t << std::endl;
		std::cout << "Ray-Sphere nearest " << b3 << " " << b3t << std::endl;
		std::cout << "Ray-Plane nearest " << b4 << " " << b4t << std::endl;
		std::cout << "Ray-Triangle nearest " << b5 << " " << b5t << std::endl;

	}


	void Benchmarks::operator()() const
	{
		HANDLE thisProcess = GetCurrentProcess();
		DWORD_PTR originalAffinityMask, t;
		GetProcessAffinityMask(thisProcess, &originalAffinityMask, &t);
		SetProcessAffinityMask(thisProcess, 0);

		runBenchmarks();

		SetProcessAffinityMask(thisProcess, originalAffinityMask);
	}

}

