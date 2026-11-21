#pragma once

#include "../KDTreeTraverser.h"

#include "raytracing/common/StatisticsCookie.h"

#include <array>
#include <cassert>

namespace raytracer
{
    using namespace vectorization;
    using namespace primitives;

    // Traverses the four-wide view of the tree (KDTreeRoot::wideNodes): one SIMD test decides for up to four children,
    // so a ray visits about half as many nodes as with NaiveKDTreeTraverser.
    // - Nearest queries go into the hit child the ray enters first and defer the others, nearer ones on top. A deferred
    //   child whose entry distance lies beyond a hit found in the meantime is dropped when it is popped.
    // - Any-hit queries take the hit children in lane order, depth-first: the leaves come in the same order as with
    //   NaiveKDTreeTraverser, which takes child A first.
    template <typename IntersectionInfoType>
    class WideKDTreeTraverser : public KDTreeTraverser<IntersectionInfoType>
    {
        // a wide node defers up to 3 children and covers two levels of the tree
        static constexpr ASizeT StackSize = 2 * KDTreeRoot::MaxDepth;

    public:
        ~WideKDTreeTraverser() override = default;

        Float findNearestIntersection(
            const GeometryNodesTraverser<IntersectionInfoType>& geometryNodesTraverser, const KDTreeRoot& root,
            const RayCast& rayCast, const IntersectionInfoType* const originIntersection,
            IntersectionInfoType& intersectionOut
        ) const override
        {
            if (!enterRoot(rayCast, root))
            {
                return rayCast.maxDistance;
            }
            RayCast rayCastOut = rayCast;
            const auto ray = KDTreeWideRay(rayCast.ray);

            struct Deferred
            {
                KDTreeLink link;
                Float entryDistance;
            };
            std::array<Deferred, StackSize> deferred;
            auto deferredCount = ASizeT{0};
            KDTreeLink traversal = root.wideRootLink;

            while (true)
            {
                if (KDTreeRoot::isLeaf(traversal))
                {
                    rayCastOut.maxDistance = geometryNodesTraverser.findNearestIntersection(
                        root.geometryOf(traversal), rayCastOut, originIntersection, intersectionOut
                    );
                }
                else
                {
                    if (StatisticsCookie::current)
                    {
                        ++StatisticsCookie::current->kdTreeNodesVisited;
                    }
                    const auto& node = root.wideNodes[traversal];
                    auto entryDistances = Float4();
                    const auto hits = overlappedLanes(rayCastOut, ray, node, entryDistances);

                    if (hits != 0)
                    {
                        // the hit lanes, sorted by entry distance (lane order on a tie)
                        std::array<Deferred, 4> sorted;
                        auto count = ASizeT{0};
                        for (auto lane = 0; lane < 4; ++lane)
                        {
                            if (hits & (1u << lane))
                            {
                                const auto candidate = Deferred{node.links[lane], entryDistances[lane]};
                                auto position = count++;
                                for (; position > 0 && candidate.entryDistance < sorted[position - 1].entryDistance;
                                       --position)
                                {
                                    sorted[position] = sorted[position - 1];
                                }
                                sorted[position] = candidate;
                            }
                        }

                        // the nearest one next, the others deferred with the farthest at the bottom
                        assert(deferredCount + count <= deferred.size());
                        for (auto i = count; i-- > 1;)
                        {
                            deferred[deferredCount++] = sorted[i];
                        }
                        traversal = sorted[0].link;
                        continue;
                    }
                }

                // a hit in the meantime may lie in front of a deferred child's box
                while (true)
                {
                    if (deferredCount == 0)
                    {
                        return rayCastOut.maxDistance;
                    }
                    const auto& next = deferred[--deferredCount];
                    if (next.entryDistance < rayCastOut.maxDistance)
                    {
                        traversal = next.link;
                        break;
                    }
                }
            }
        }

        Float findAnyIntersection(
            const GeometryNodesTraverser<IntersectionInfoType>& geometryNodesTraverser, const KDTreeRoot& root,
            const RayCast& rayCast, const IntersectionInfoType* const originIntersection,
            IntersectionInfoType& intersectionOut
        ) const override
        {
            if (!enterRoot(rayCast, root))
            {
                return rayCast.maxDistance;
            }
            RayCast rayCastOut = rayCast;
            const auto ray = KDTreeWideRay(rayCast.ray);

            std::array<KDTreeLink, StackSize> deferred;
            auto deferredCount = ASizeT{0};
            KDTreeLink traversal = root.wideRootLink;

            while (true)
            {
                if (KDTreeRoot::isLeaf(traversal))
                {
                    rayCastOut.maxDistance = geometryNodesTraverser.findAnyIntersection(
                        root.geometryOf(traversal), rayCastOut, originIntersection, intersectionOut
                    );

                    // short circuit on the first hit
                    if (!outOfReach(rayCast, rayCastOut.maxDistance))
                    {
                        return rayCastOut.maxDistance;
                    }
                }
                else
                {
                    if (StatisticsCookie::current)
                    {
                        ++StatisticsCookie::current->kdTreeNodesVisited;
                    }
                    const auto& node = root.wideNodes[traversal];
                    const auto hits = overlappedLanes(rayCastOut, ray, node);

                    // in reverse, so that lane 0 comes next
                    assert(deferredCount + 4 <= deferred.size());
                    for (auto lane = 4; lane-- > 0;)
                    {
                        if (hits & (1u << lane))
                        {
                            deferred[deferredCount++] = node.links[lane];
                        }
                    }
                }

                if (deferredCount == 0)
                {
                    return rayCastOut.maxDistance;
                }
                traversal = deferred[--deferredCount];
            }
        }

    private:
        // true if the ray overlaps the root's box; counts the root as a visited node like NaiveKDTreeTraverser
        static bool enterRoot(const RayCast& rayCast, const KDTreeRoot& root) noexcept
        {
            if (!overlaps(rayCast, root.rootBounding))
            {
                if (StatisticsCookie::current)
                {
                    ++StatisticsCookie::current->sceneRootOverlapMisses;
                }
                return false;
            }
            if (StatisticsCookie::current)
            {
                ++StatisticsCookie::current->kdTreeNodesVisited;
            }
            return true;
        }
    };
}
