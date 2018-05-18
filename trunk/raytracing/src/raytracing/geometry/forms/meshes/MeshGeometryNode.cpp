#include "raytracing/geometry/forms/meshes/MeshGeometryNode.h"
#include "../../../../stdafx.h"

namespace raytracer
{

#pragma region privates
	//{privates

	const bool planeBoxOverlap(const Float4 & normal, const Float4 & vert, const Float4 & maxbox)
	{
		const Float4::VectorBoolType mask = (normal > Zero<Float4>());
		const Float4 a = maxbox - vert;
		const Float4 b = -maxbox - vert;
		const Float4 vmin = blendMasked(a, b, mask);
		const Float4 vmax = blendMasked(b, a, mask);
		const Float4::VectorBoolType vminSide = dot3v(normal, vmin) <= Zero<Float4>();
		const Float4::VectorBoolType vmaxSide = dot3v(normal, vmax) >= Zero<Float4>();
		return allTrue(vminSide & vmaxSide);
	}

	const bool axisTest(
		const Float4 & edge,
		const Float4 & yzxBoxHalfSize,
		const Float4 & zxyBoxHalfSize,
		const Float4 & w00,
		const Float4 & w01,
		const Float4 & w10,
		const Float4 & w11
	)
	{
		const Float4 fe = abs(edge);
		const Float4 rad = zxyw(fe) * yzxBoxHalfSize + yzxw(fe) * zxyBoxHalfSize;
		const Float4 zxyE = zxyw(edge);
		const Float4 yzxE = yzxw(edge);
		const Float4 p135 = zxyE * w00 - yzxE * w01;
		const Float4 p246 = zxyE * w10 - yzxE * w11;
		return anyTrue3(min(p135, p246) > rad | max(p135, p246) < -rad);
	}

	const bool triBoxOverlap(const AxisAlignedBoundingBox & aabb, const Facet & trianglePlanes)
	{
		// based on Fast 3D Triangle-Box Overlap Testing by Tomas Akenine-Moller
		// https://fileadmin.cs.lth.se/cs/Personal/Tomas_Akenine-Moller/pubs/tribox.pdf
		// tests are in their SSE equivalent manner

		// case 1: Test the AABB against the minimal AABB around the triangle
		const AxisAlignedBoundingBox triBox = bounding(trianglePlanes);
		if (!overlaps(aabb, triBox)) {
			return false;
		}

		// case 2: fast plane/AABB overlap test, which only tests the two diagonal
		// vertices, whose direction is most closely aligned to the normal of
		// the triangle
		const Float4 f0 = trianglePlanes.v1 - trianglePlanes.v0;
		const Float4 f1 = trianglePlanes.v2 - trianglePlanes.v1;
		const Float4 n = cross3(f0, f1);
		const Float4 boxHalfSize = halfLengths(aabb);
		const Float4 boxCenter = center(aabb, boxHalfSize);
		const Float4 v0 = trianglePlanes.v0 - boxCenter;
		if (!planeBoxOverlap(n, v0, boxHalfSize)) {
			return false;
		}

		// case 3: 9 SAT (separating axis theorem) tests
		// a_ij = cross(e_i, f_j) and i and j in {0, 1, 2},
		// where f0 = v1 - v0, f1 = v2 - v1 and f2 = v0 - v2
		// and e_i are the axis aligned normals of the AABB, f.e. e_0 = (1, 0, 0)

		// outline of the test for i=j=0:
		// a_00 = e_0 * f_0 = (0, -f_0.z, f_0.y)

		const Float4 v1 = trianglePlanes.v1 - boxCenter;
		const Float4 v2 = trianglePlanes.v2 - boxCenter;
		const Float4 yzxV0 = yzxw(v0);
		const Float4 zxyV0 = zxyw(v0);
		const Float4 yyyV1 = yyyy(v1);
		const Float4 yzxV2 = yzxw(v2);
		const Float4 zxyV2 = zxyw(v2);
		const Float4 yzxBoxHalfSize = yzxw(boxHalfSize);
		const Float4 zxyBoxHalfSize = zxyw(boxHalfSize);

		if (axisTest(
			f0,
			yzxBoxHalfSize,
			zxyBoxHalfSize,
			xy_xy(yzxV0, v1),
			xy_xy(zxyV0, yyyV1),
			yzxV2,
			zxyV2
		)) {
			return false;
		}

		if (axisTest(
			f1,
			yzxBoxHalfSize,
			zxyBoxHalfSize,
			yzxV0,
			zxyV0,
			xy_xy(yzxV2, v1),
			xy_xy(zxyV2, yyyV1)
		)) {
			return false;
		}

		if (axisTest(
			v0 - v2,
			yzxBoxHalfSize,
			zxyBoxHalfSize,
			xy_xy(yzxV0, v1),
			xy_xy(zxyV0, yyyV1),
			xy_xy(yzxw(v1), v2),
			xy_xy(zxyw(v1), yyyy(v2))
		)) {
			return false;
		}

		return true;
	}

	//}
#pragma endregion

	MeshGeometryNode::MeshGeometryNode()
		:
		index(),
		trianglePlanes()
	{ }

	MeshGeometryNode::MeshGeometryNode(const ASizeT index, const Facet & trianglePlanes)
		:
		index(index),
		trianglePlanes(trianglePlanes)
	{ }

	// GeometryNode interface

	void MeshGeometryNode::includeInBounding(AxisAlignedBoundingBox & aabb) const
	{
		aabb = extendBy(
			extendBy(
				extendBy(aabb, trianglePlanes.v0),
				trianglePlanes.v1
			),
			trianglePlanes.v2
		);
	}

	const bool MeshGeometryNode::overlaps(const AxisAlignedBoundingBox & aabb) const
	{
		return triBoxOverlap(aabb, trianglePlanes);
	}

	// Intersectable<Raycast, FacetIntersection> interface

	const Float MeshGeometryNode::findNearestIntersection(const Raycast & ray, const FacetIntersection * const, FacetIntersection &) const
	{
		return ray.maxDistance;
	}

	const Float MeshGeometryNode::findAnyIntersection(const Raycast & ray, const FacetIntersection * const, FacetIntersection &) const
	{
		return ray.maxDistance;
	}

}
