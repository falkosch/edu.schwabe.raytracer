#pragma once

#include "../KDTreeTraverser.h"

namespace raytracer {
  using namespace vectorization;
  using namespace primitives;

  template <typename IntersectionInfoType>
  class VoxelizationKDTreeTraverser : public KDTreeTraverser<IntersectionInfoType> {
    static void computeFacetIntersection(
        const Float d, const AxisAlignedBoundingBox &bounding, RayCast &rayCast, FacetIntersection &intersectionOut
    ) {
      if (outOfReach(rayCast, d)) {
        return;
      }

      rayCast.maxDistance = d;

      const Float4 msVertex = point(rayCast.ray, d);
      intersectionOut.msVertex = msVertex;

      const Float4 orientation = msVertex - center(bounding);
      const Float4 surfaceNormal = axisAlignedNormal3(orientation);
      intersectionOut.msSurfaceNormal = surfaceNormal;
      intersectionOut.vertex = msVertex;
      intersectionOut.surfaceNormal = surfaceNormal;

      const Float4 smoothedNormal = normalize(orientation);
      intersectionOut.smoothedNormal = smoothedNormal;
      intersectionOut.texCoords = mapSpherical(smoothedNormal);
      intersectionOut.node = nullptr;
    }

    static void findNearestIntersection(
        const GeometryNodesTraverser<IntersectionInfoType> &geometryNodesTraverser, const KDTreeNode &node,
        const AxisAlignedBoundingBox &nodeBounding, const IntersectionInfoType *const originIntersection, RayCast &tr,
        IntersectionInfoType &intersectionOut
    ) {
      // Traverse children
      const AxisAlignedBoundingBox *lastHitBounding = &nodeBounding;
      const KDTreeNode *traversal = &node;

      while (traversal->children) {
        const KDTreeNodeChildren &children = *traversal->children;
        const Float4::VectorBoolType leftRightHit = overlaps(tr, children.boundingA, children.boundingB);

        if (allTrue(leftRightHit)) {
          // Set right as next iteration step
          lastHitBounding = &children.boundingB;
          traversal = &children.childB;
          // We need a recursive call to test the left node. (depth-first traversal)
          findNearestIntersection(
              geometryNodesTraverser, children.childA, children.boundingA, originIntersection, tr, intersectionOut
          );
          continue;
        }

        // select one of the nodes as next iteration step
        if (x(leftRightHit)) {
          traversal = &children.childA;
          lastHitBounding = &children.boundingA;
        } else {
          traversal = &children.childB;
          lastHitBounding = &children.boundingB;
        }
      }

      // At a non-empty leaf node, its geometry nodes are tested for intersection.
      assert(traversal->isNonEmptyLeaf());
      computeFacetIntersection(
          nearestIntersection(tr, *lastHitBounding, reinterpret_cast<ASizeT>(lastHitBounding)), *lastHitBounding, tr,
          intersectionOut
      );
    }

    static void findAnyIntersection(
        const GeometryNodesTraverser<IntersectionInfoType> &geometryNodesTraverser, const KDTreeNode &node,
        const AxisAlignedBoundingBox &nodeBounding, const RayCast &rayCast,
        const IntersectionInfoType *const originIntersection, RayCast &tr, IntersectionInfoType &intersectionOut
    ) {
      // Traverse children
      const AxisAlignedBoundingBox *lastHitBounding = &nodeBounding;
      const KDTreeNode *traversal = &node;

      while (traversal->children) {
        const KDTreeNodeChildren &children = *traversal->children;
        const Float4::VectorBoolType leftRightHit = overlaps(tr, children.boundingA, children.boundingB);

        if (allTrue(leftRightHit)) {
          // Set right as next iteration step
          lastHitBounding = &children.boundingB;
          traversal = &children.childB;
          // We need a recursive call to test the left node. (depth-first traversal)
          findAnyIntersection(
              geometryNodesTraverser, children.childA, children.boundingA, rayCast, originIntersection, tr,
              intersectionOut
          );

          // Short circuit the recursion on hit
          if (!outOfReach(rayCast, tr.maxDistance)) {
            return;
          }

          // Or continue with iteration on childB
          continue;
        }

        // select one of the nodes as next iteration step
        if (x(leftRightHit)) {
          traversal = &children.childA;
          lastHitBounding = &children.boundingA;
        } else {
          traversal = &children.childB;
          lastHitBounding = &children.boundingB;
        }
      }

      // At a non-empty leaf node, its geometry nodes are tested for intersection.
      assert(traversal->isNonEmptyLeaf());
      computeFacetIntersection(
          nearestIntersection(tr, *lastHitBounding, reinterpret_cast<ASizeT>(lastHitBounding)), *lastHitBounding, tr,
          intersectionOut
      );
    }

  public:
    virtual ~VoxelizationKDTreeTraverser() {
    }

    const Float findNearestIntersection(
        const GeometryNodesTraverser<IntersectionInfoType> &geometryNodesTraverser, const KDTreeRoot &root,
        const RayCast &rayCast, const IntersectionInfoType *const originIntersection,
        IntersectionInfoType &intersectionOut
    ) const {
      // Do we have a hit at root?
      if (!overlaps(rayCast, root.rootBounding)) {
        return rayCast.maxDistance;
      }

      RayCast tr = rayCast;
      findNearestIntersection(
          geometryNodesTraverser, root.rootNode, root.rootBounding, originIntersection, tr, intersectionOut
      );
      return tr.maxDistance;
    }

    const Float findAnyIntersection(
        const GeometryNodesTraverser<IntersectionInfoType> &geometryNodesTraverser, const KDTreeRoot &root,
        const RayCast &rayCast, const IntersectionInfoType *const originIntersection,
        IntersectionInfoType &intersectionOut
    ) const {
      // Do we have a hit at root?
      if (!overlaps(rayCast, root.rootBounding)) {
        return rayCast.maxDistance;
      }

      RayCast rayCastOut = rayCast;
      findAnyIntersection(
          geometryNodesTraverser, root.rootNode, root.rootBounding, rayCast, originIntersection, rayCastOut,
          intersectionOut
      );
      return rayCastOut.maxDistance;
    }
  };
}
