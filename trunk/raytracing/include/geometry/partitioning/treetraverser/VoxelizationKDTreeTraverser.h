#pragma once

#include "geometry/partitioning/KDTreeTraverser.h"

namespace raytracer
{

    template <typename IntersectionInfoType>
    class VoxelizationKDTreeTraverser : public KDTreeTraverser < IntersectionInfoType >
    {

        static void computeFacetIntersection(
            const Float d,
            const AxisAlignedBoundingBox & bounding,
            Raycast & r,
            FacetIntersection & intersectionOut)
        {
            if (outOfReach(r, d)) return;
            r.maxDistance = d;

            const Float4 msVertex = point(r.ray, d);
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
            const GeometryNodesTraverser<IntersectionInfoType> & geometryNodesTraverser,
            const KDTreeNode & node,
            const AxisAlignedBoundingBox & nodeBounding,
            const IntersectionInfoType * const originIntersection,
            Raycast & tr,
            IntersectionInfoType & intersectionOut)
        {
            // Traverse childs
            const AxisAlignedBoundingBox *lastHitBounding = &nodeBounding;
            const KDTreeNode *traversel = &node;
            while (traversel->childs)
            {
                const KDTreeNodeChilds & childs = *traversel->childs;
                const Float4::VectorBoolType leftRightHit = overlaps(tr, childs.boundingA, childs.boundingB);

                if (allTrue(leftRightHit))
                {
                    // Set right as next iteration step
                    lastHitBounding = &childs.boundingB;
                    traversel = &childs.childB;
                    // We need a recursive call to test the left node. (depth-first traversal)
                    findNearestIntersection(geometryNodesTraverser, childs.childA, childs.boundingA, originIntersection, tr, intersectionOut);
                    continue;
                }

                // select one of the nodes as next iteration step
                if (x(leftRightHit))
                {
                    traversel = &childs.childA;
                    lastHitBounding = &childs.boundingA;
                }
                else
                {
                    traversel = &childs.childB;
                    lastHitBounding = &childs.boundingB;
                }
            }

            // At a non-empty leaf node, its geometry nodes are tested for intersection.
            assert(traversel->isNonEmptyLeaf());
            computeFacetIntersection(
                nearestIntersection(tr, *lastHitBounding, reinterpret_cast<ASizeT>(lastHitBounding)),
                *lastHitBounding,
                tr,
                intersectionOut);
        }

        static void findAnyIntersection(
            const GeometryNodesTraverser<IntersectionInfoType> & geometryNodesTraverser,
            const KDTreeNode & node,
            const AxisAlignedBoundingBox & nodeBounding,
            const Raycast & r,
            const IntersectionInfoType * const originIntersection,
            Raycast & tr,
            IntersectionInfoType & intersectionOut)
        {
            // Traverse childs
            const AxisAlignedBoundingBox *lastHitBounding = &nodeBounding;
            const KDTreeNode *traversel = &node;
            while (traversel->childs)
            {
                const KDTreeNodeChilds & childs = *traversel->childs;
                const Float4::VectorBoolType leftRightHit = overlaps(tr, childs.boundingA, childs.boundingB);

                if (allTrue(leftRightHit))
                {
                    // Set right as next iteration step
                    lastHitBounding = &childs.boundingB;
                    traversel = &childs.childB;
                    // We need a recursive call to test the left node. (depth-first traversal)
                    findAnyIntersection(geometryNodesTraverser, childs.childA, childs.boundingA, r, originIntersection, tr, intersectionOut);
                    // Short circuit the recursion on hit
                    if (!outOfReach(r, tr.maxDistance)) return;
                    // Or continue with iteration on childB
                    continue;
                }

                // select one of the nodes as next iteration step
                if (x(leftRightHit))
                {
                    traversel = &childs.childA;
                    lastHitBounding = &childs.boundingA;
                }
                else
                {
                    traversel = &childs.childB;
                    lastHitBounding = &childs.boundingB;
                }
            }

            // At a non-empty leaf node, its geometry nodes are tested for intersection.
            assert(traversel->isNonEmptyLeaf());
            computeFacetIntersection(
                nearestIntersection(tr, *lastHitBounding, reinterpret_cast<ASizeT>(lastHitBounding)),
                *lastHitBounding,
                tr,
                intersectionOut);
        }

    public:

        virtual ~VoxelizationKDTreeTraverser() { }

        const Float findNearestIntersection(
            const GeometryNodesTraverser<IntersectionInfoType> & geometryNodesTraverser,
            const KDTreeRoot & root,
            const Raycast & r,
            const IntersectionInfoType * const originIntersection,
            IntersectionInfoType & intersectionOut) const
        {
            // Do we have a hit at root?
            if (!overlaps(r, root.rootBounding)) return r.maxDistance;

            Raycast tr = r;
            findNearestIntersection(geometryNodesTraverser, root.rootNode, root.rootBounding, originIntersection, tr, intersectionOut);
            return tr.maxDistance;
        }

        const Float findAnyIntersection(
            const GeometryNodesTraverser<IntersectionInfoType> & geometryNodesTraverser,
            const KDTreeRoot & root,
            const Raycast & r,
            const IntersectionInfoType * const originIntersection,
            IntersectionInfoType & intersectionOut) const
        {
            // Do we have a hit at root?
            if (!overlaps(r, root.rootBounding)) return r.maxDistance;

            Raycast tr = r;
            findAnyIntersection(geometryNodesTraverser, root.rootNode, root.rootBounding, r, originIntersection, tr, intersectionOut);
            return tr.maxDistance;
        }

    };

}
