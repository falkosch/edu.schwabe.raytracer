#pragma once

#include "../KDTreeTraverser.h"

#include "raytracing/common/StatisticsCookie.h"

#include <array>
#include <cassert>

namespace raytracer
{
    using namespace vectorization;
    using namespace primitives;

    // Depth-first traversal with an explicit stack: where the ray hits both children, child A is traversed first and
    // child B is deferred until A's subtree is done.
    template <typename IntersectionInfoType>
    class NaiveKDTreeTraverser : public KDTreeTraverser<IntersectionInfoType>
    {
    public:
        ~NaiveKDTreeTraverser() override = default;

        Float findNearestIntersection(
            const GeometryNodesTraverser<IntersectionInfoType>& geometryNodesTraverser, const KDTreeRoot& root,
            const RayCast& rayCast, const IntersectionInfoType* const originIntersection,
            IntersectionInfoType& intersectionOut
        ) const override
        {
            if (!overlaps(rayCast, root.rootBounding))
            {
                if (StatisticsCookie::current)
                {
                    ++StatisticsCookie::current->sceneRootOverlapMisses;
                }
                return rayCast.maxDistance;
            }

            if (StatisticsCookie::current)
            {
                ++StatisticsCookie::current->kdTreeNodesVisited;
            }
            RayCast rayCastOut = rayCast;

            std::array<KDTreeLink, KDTreeRoot::MaxDepth> deferred;
            auto deferredCount = ASizeT{0};
            KDTreeLink traversal = root.rootLink;

            while (true)
            {
                while (!KDTreeRoot::isLeaf(traversal))
                {
                    if (StatisticsCookie::current)
                    {
                        ++StatisticsCookie::current->kdTreeNodesVisited;
                    }
                    const KDTreeChildBoundings& boundings = root.childBoundings[traversal];
                    const KDTreeChildLinks& children = root.childLinks[traversal];
                    const auto leftRightHit = overlaps(rayCastOut, boundings.boundingA, boundings.boundingB);

                    if (allTrue(leftRightHit))
                    {
                        // child A first, child B once A's subtree is done
                        assert(deferredCount < deferred.size());
                        deferred[deferredCount++] = children.childB;
                        traversal = children.childA;
                        continue;
                    }

                    // select one of the nodes as next iteration step
                    traversal = x(leftRightHit) ? children.childA : children.childB;
                }

                // At a leaf node, its geometry nodes are tested for intersection. A hit shortens rayCastOut, which
                // prunes the deferred children.
                rayCastOut.maxDistance = geometryNodesTraverser.findNearestIntersection(
                    root.geometryOf(traversal), rayCastOut, originIntersection, intersectionOut
                );

                if (deferredCount == 0)
                {
                    return rayCastOut.maxDistance;
                }
                traversal = deferred[--deferredCount];
            }
        }

        Float findAnyIntersection(
            const GeometryNodesTraverser<IntersectionInfoType>& geometryNodesTraverser, const KDTreeRoot& root,
            const RayCast& rayCast, const IntersectionInfoType* const originIntersection,
            IntersectionInfoType& intersectionOut
        ) const override
        {
            if (!overlaps(rayCast, root.rootBounding))
            {
                if (StatisticsCookie::current)
                {
                    ++StatisticsCookie::current->sceneRootOverlapMisses;
                }
                return rayCast.maxDistance;
            }

            if (StatisticsCookie::current)
            {
                ++StatisticsCookie::current->kdTreeNodesVisited;
            }
            RayCast rayCastOut = rayCast;

            std::array<KDTreeLink, KDTreeRoot::MaxDepth> deferred;
            auto deferredCount = ASizeT{0};
            KDTreeLink traversal = root.rootLink;

            while (true)
            {
                while (!KDTreeRoot::isLeaf(traversal))
                {
                    if (StatisticsCookie::current)
                    {
                        ++StatisticsCookie::current->kdTreeNodesVisited;
                    }
                    const KDTreeChildBoundings& boundings = root.childBoundings[traversal];
                    const KDTreeChildLinks& children = root.childLinks[traversal];
                    const auto leftRightHit = overlaps(rayCastOut, boundings.boundingA, boundings.boundingB);

                    if (allTrue(leftRightHit))
                    {
                        // child A first, child B once A's subtree is done
                        assert(deferredCount < deferred.size());
                        deferred[deferredCount++] = children.childB;
                        traversal = children.childA;
                        continue;
                    }

                    // select one of the nodes as next iteration step
                    traversal = x(leftRightHit) ? children.childA : children.childB;
                }

                // At a leaf node, its geometry nodes are tested for intersection.
                rayCastOut.maxDistance = geometryNodesTraverser.findAnyIntersection(
                    root.geometryOf(traversal), rayCastOut, originIntersection, intersectionOut
                );

                // short circuit on the first hit
                if (!outOfReach(rayCast, rayCastOut.maxDistance) || deferredCount == 0)
                {
                    return rayCastOut.maxDistance;
                }
                traversal = deferred[--deferredCount];
            }
        }
    };
}
