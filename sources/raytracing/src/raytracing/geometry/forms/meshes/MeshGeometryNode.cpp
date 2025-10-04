#include "raytracing/geometry/forms/meshes/MeshGeometryNode.h"
#include "../../../../stdafx.h"

namespace raytracer {
  const bool planeBoxOverlap(const Float4 &normal, const Float4 &vert, const Float4 &maxbox) {
    const auto mask = normal > Zero<Float4>();
    const auto a = maxbox - vert;
    const auto b = -maxbox - vert;
    const auto vmin = blendMasked(a, b, mask);
    const auto vmax = blendMasked(b, a, mask);
    const auto vminSide = dot3v(normal, vmin) <= Zero<Float4>();
    const auto vmaxSide = dot3v(normal, vmax) >= Zero<Float4>();
    return allTrue(vminSide & vmaxSide);
  }

  const bool axisTest(
      const Float4 &edge, const Float4 &yzxBoxHalfSize, const Float4 &zxyBoxHalfSize, const Float4 &w00,
      const Float4 &w01, const Float4 &w10, const Float4 &w11
  ) {
    const auto fe = abs(edge);
    const auto rad = zxyw(fe) * yzxBoxHalfSize + yzxw(fe) * zxyBoxHalfSize;
    const auto zxyE = zxyw(edge);
    const auto yzxE = yzxw(edge);
    const auto p135 = zxyE * w00 - yzxE * w01;
    const auto p246 = zxyE * w10 - yzxE * w11;
    return anyTrue3((min(p135, p246) > rad) | (max(p135, p246) < -rad));
  }

  const bool triBoxOverlap(const AxisAlignedBoundingBox &aabb, const Facet &trianglePlanes) {
    // based on Fast 3D Triangle-Box Overlap Testing by Tomas Akenine-Moller
    // https://fileadmin.cs.lth.se/cs/Personal/Tomas_Akenine-Moller/pubs/tribox.pdf
    // tests are in their SSE equivalent manner

    // case 1: Test the AABB against the minimal AABB around the triangle
    auto triBox = bounding(trianglePlanes);
    if (!overlaps(aabb, triBox)) {
      return false;
    }

    // case 2: fast plane/AABB overlap test, which only tests the two diagonal
    // vertices, whose direction is most closely aligned to the normal of
    // the triangle
    auto f0 = trianglePlanes.v1 - trianglePlanes.v0;
    auto f1 = trianglePlanes.v2 - trianglePlanes.v1;
    auto n = cross3(f0, f1);
    auto boxHalfSize = halfLengths(aabb);
    auto boxCenter = center(aabb, boxHalfSize);
    auto v0 = trianglePlanes.v0 - boxCenter;
    if (!planeBoxOverlap(n, v0, boxHalfSize)) {
      return false;
    }

    // case 3: 9 SAT (separating axis theorem) tests
    // a_ij = cross(e_i, f_j) and i and j in {0, 1, 2},
    // where f0 = v1 - v0, f1 = v2 - v1 and f2 = v0 - v2
    // and e_i are the axis aligned normals of the AABB, f.e. e_0 = (1, 0, 0)

    // outline of the test for i=j=0:
    // a_00 = e_0 * f_0 = (0, -f_0.z, f_0.y)

    auto v1 = trianglePlanes.v1 - boxCenter;
    auto v2 = trianglePlanes.v2 - boxCenter;
    auto yzxV0 = yzxw(v0);
    auto zxyV0 = zxyw(v0);
    auto yyyV1 = yyyy(v1);
    auto yzxV2 = yzxw(v2);
    auto zxyV2 = zxyw(v2);
    auto yzxBoxHalfSize = yzxw(boxHalfSize);
    auto zxyBoxHalfSize = zxyw(boxHalfSize);

    if (axisTest(f0, yzxBoxHalfSize, zxyBoxHalfSize, xy_xy(yzxV0, v1), xy_xy(zxyV0, yyyV1), yzxV2, zxyV2)) {
      return false;
    }

    if (axisTest(f1, yzxBoxHalfSize, zxyBoxHalfSize, yzxV0, zxyV0, xy_xy(yzxV2, v1), xy_xy(zxyV2, yyyV1))) {
      return false;
    }

    if (axisTest(
            v0 - v2, yzxBoxHalfSize, zxyBoxHalfSize, xy_xy(yzxV0, v1), xy_xy(zxyV0, yyyV1), xy_xy(yzxw(v1), v2),
            xy_xy(zxyw(v1), yyyy(v2))
        )) {
      return false;
    }

    return true;
  }

  MeshGeometryNode::MeshGeometryNode() : index(), facet() {
  }

  MeshGeometryNode::MeshGeometryNode(const ASizeT index, const Facet &facet) : index(index), facet(facet) {
  }

  // GeometryNode interface

  const AxisAlignedBoundingBox MeshGeometryNode::includeInBounding(const AxisAlignedBoundingBox &aabb) const {
    return extendBy(extendBy(extendBy(aabb, facet.v0), facet.v1), facet.v2);
  }

  const bool MeshGeometryNode::overlaps(const AxisAlignedBoundingBox &aabb) const {
    return triBoxOverlap(aabb, facet);
  }

  // Intersectable<RayCast, FacetIntersection> interface

  const Float
  MeshGeometryNode::findNearestIntersection(const RayCast &ray, const FacetIntersection *const, FacetIntersection &)
      const {
    return ray.maxDistance;
  }

  const Float
  MeshGeometryNode::findAnyIntersection(const RayCast &ray, const FacetIntersection *const, FacetIntersection &) const {
    return ray.maxDistance;
  }
}
