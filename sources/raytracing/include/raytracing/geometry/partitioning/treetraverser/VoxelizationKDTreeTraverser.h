#pragma once

#include "../KDTreeTraverser.h"

#include "raytracing/common/StatisticsCookie.h"

#include <array>
#include <cassert>

namespace raytracer
{
  using namespace vectorization;
  using namespace primitives;

  template <typename IntersectionInfoType>
  class VoxelizationKDTreeTraverser : public KDTreeTraverser<IntersectionInfoType>
  {
    static void computeFacetIntersection(
      const Float d, const AxisAlignedBoundingBox& bounding, RayCast& rayCast, FacetIntersection& intersectionOut
    )
    {
      if (outOfReach(rayCast, d))
      {
        return;
      }

      rayCast.maxDistance = d;

      const auto msVertex = point(rayCast.ray, d);
      intersectionOut.msVertex = msVertex;

      const auto orientation = msVertex - center(bounding);
      const auto surfaceNormal = axisAlignedNormal3(orientation);
      intersectionOut.msSurfaceNormal = surfaceNormal;
      intersectionOut.vertex = msVertex;
      intersectionOut.surfaceNormal = surfaceNormal;

      const auto smoothedNormal = normalize(orientation);
      intersectionOut.smoothedNormal = smoothedNormal;
      intersectionOut.texCoords = mapSpherical(smoothedNormal);
      intersectionOut.node = nullptr;
    }

    // child B and its bounding, deferred while child A's subtree is traversed
    struct DeferredChild
    {
      KDTreeLink link;
      const AxisAlignedBoundingBox* bounding;
    };

    // Depth-first traversal with an explicit stack. At each leaf, the leaf's bounding is intersected instead of its
    // geometry (debug view of the voxels). With stopAtFirstHit, it ends at the first leaf the ray reaches within
    // rayCast.maxDistance.
    static void traverse(
      const KDTreeRoot& root, const RayCast& rayCast, const bool stopAtFirstHit, RayCast& tr,
      IntersectionInfoType& intersectionOut
    )
    {
      std::array<DeferredChild, KDTreeRoot::MaxDepth> deferred;
      auto deferredCount = ASizeT{0};

      const AxisAlignedBoundingBox* lastHitBounding = &root.rootBounding;
      KDTreeLink traversal = root.rootLink;

      while (true)
      {
        while (!KDTreeRoot::isLeaf(traversal))
        {
          if (StatisticsCookie::current) ++StatisticsCookie::current->kdTreeNodesVisited;
          const KDTreeChildBoundings& boundings = root.childBoundings[traversal];
          const KDTreeChildLinks& children = root.childLinks[traversal];
          const Float4::VectorBoolType leftRightHit = overlaps(tr, boundings.boundingA, boundings.boundingB);

          if (allTrue(leftRightHit))
          {
            // child A first, child B once A's subtree is done
            assert(deferredCount < deferred.size());
            deferred[deferredCount++] = DeferredChild{children.childB, &boundings.boundingB};
            traversal = children.childA;
            lastHitBounding = &boundings.boundingA;
            continue;
          }

          // select one of the nodes as next iteration step
          if (x(leftRightHit))
          {
            traversal = children.childA;
            lastHitBounding = &boundings.boundingA;
          }
          else
          {
            traversal = children.childB;
            lastHitBounding = &boundings.boundingB;
          }
        }

        computeFacetIntersection(
          nearestIntersection(tr, *lastHitBounding, reinterpret_cast<ASizeT>(lastHitBounding)), *lastHitBounding, tr,
          intersectionOut
        );

        if ((stopAtFirstHit && !outOfReach(rayCast, tr.maxDistance)) || deferredCount == 0)
        {
          return;
        }
        --deferredCount;
        traversal = deferred[deferredCount].link;
        lastHitBounding = deferred[deferredCount].bounding;
      }
    }

  public:
    ~VoxelizationKDTreeTraverser() override = default;

    Float findNearestIntersection(
      const GeometryNodesTraverser<IntersectionInfoType>& /*geometryNodesTraverser*/, const KDTreeRoot& root,
      const RayCast& rayCast, const IntersectionInfoType* const /*originIntersection*/,
      IntersectionInfoType& intersectionOut
    ) const override
    {
      if (!overlaps(rayCast, root.rootBounding))
      {
        return rayCast.maxDistance;
      }

      if (StatisticsCookie::current) ++StatisticsCookie::current->kdTreeNodesVisited;
      RayCast tr = rayCast;
      traverse(root, rayCast, false, tr, intersectionOut);
      return tr.maxDistance;
    }

    Float findAnyIntersection(
      const GeometryNodesTraverser<IntersectionInfoType>& /*geometryNodesTraverser*/, const KDTreeRoot& root,
      const RayCast& rayCast, const IntersectionInfoType* const /*originIntersection*/,
      IntersectionInfoType& intersectionOut
    ) const override
    {
      if (!overlaps(rayCast, root.rootBounding))
      {
        return rayCast.maxDistance;
      }

      if (StatisticsCookie::current) ++StatisticsCookie::current->kdTreeNodesVisited;
      RayCast rayCastOut = rayCast;
      traverse(root, rayCast, true, rayCastOut, intersectionOut);
      return rayCastOut.maxDistance;
    }
  };
}
