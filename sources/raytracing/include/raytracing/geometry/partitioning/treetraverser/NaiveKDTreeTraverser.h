#pragma once

#include "../KDTreeTraverser.h"

#include <cassert>

namespace raytracer {
  using namespace vectorization;
  using namespace primitives;

  template <typename IntersectionInfoType>
  class NaiveKDTreeTraverser : public KDTreeTraverser<IntersectionInfoType> {
    static void findNearestIntersection(
        const GeometryNodesTraverser<IntersectionInfoType> &geometryNodesTraverser, const KDTreeNode &node,
        const IntersectionInfoType *const originIntersection, RayCast &rayCast, IntersectionInfoType &intersectionOut
    ) {
      // Traverse children
      const KDTreeNode *traversal = &node;

      while (traversal->children) {
        const KDTreeNodeChildren &children = *traversal->children;
        const Float4::VectorBoolType leftRightHit = overlaps(rayCast, children.boundingA, children.boundingB);

        if (allTrue(leftRightHit)) {
          // Set right as next iteration step
          traversal = &children.childB;
          // We need a recursive call to test the left node. (depth-first traversal)
          findNearestIntersection(
              geometryNodesTraverser, children.childA, originIntersection, rayCast, intersectionOut
          );
          continue;
        }

        // select one of the nodes as next iteration step
        traversal = x(leftRightHit) ? (&children.childA) : (&children.childB);
      }

      // At a non-empty leaf node, its geometry nodes are tested for intersection.
      assert(traversal->isNonEmptyLeaf());
      rayCast.maxDistance = geometryNodesTraverser.findNearestIntersection(
          *traversal->geometryNodes, rayCast, originIntersection, intersectionOut
      );
    }

    static void findAnyIntersection(
        const GeometryNodesTraverser<IntersectionInfoType> &geometryNodesTraverser, const KDTreeNode &node,
        const RayCast &rayCast, const IntersectionInfoType *const originIntersection, RayCast &rayCastOut,
        IntersectionInfoType &intersectionOut
    ) {
      // Traverse children
      const KDTreeNode *traversal = &node;

      while (traversal->children) {
        const KDTreeNodeChildren &children = *traversal->children;
        const Float4::VectorBoolType leftRightHit = overlaps(rayCastOut, children.boundingA, children.boundingB);

        if (allTrue(leftRightHit)) {
          // Set right as next iteration step
          traversal = &children.childB;
          // We need a recursive call to test the left node. (depth-first traversal)
          findAnyIntersection(
              geometryNodesTraverser, children.childA, rayCast, originIntersection, rayCastOut, intersectionOut
          );

          // Short circuit the recursion on hit
          if (!outOfReach(rayCast, rayCastOut.maxDistance)) {
            return;
          }

          // Or continue with iteration on childB
          continue;
        }

        // select one of the nodes as next iteration step
        traversal = x(leftRightHit) ? (&children.childA) : (&children.childB);
      }

      // At a non-empty leaf node, its geometry nodes are tested for intersection.
      assert(traversal->isNonEmptyLeaf());
      rayCastOut.maxDistance = geometryNodesTraverser.findAnyIntersection(
          *traversal->geometryNodes, rayCastOut, originIntersection, intersectionOut
      );
    }

  public:
    virtual ~NaiveKDTreeTraverser() {
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

      RayCast rayCastOut = rayCast;
      findNearestIntersection(geometryNodesTraverser, root.rootNode, originIntersection, rayCastOut, intersectionOut);
      return rayCastOut.maxDistance;
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
          geometryNodesTraverser, root.rootNode, rayCast, originIntersection, rayCastOut, intersectionOut
      );
      return rayCastOut.maxDistance;
    }
  };
}
