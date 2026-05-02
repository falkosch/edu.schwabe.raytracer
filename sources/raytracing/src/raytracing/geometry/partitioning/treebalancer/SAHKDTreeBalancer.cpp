#include "raytracing/geometry/partitioning/treebalancer/SAHKDTreeBalancer.h"
#include "../../../../stdafx.h"

#include <algorithm>
#include <vector>

namespace raytracer
{
    SAHKDTreeBalancer::~SAHKDTreeBalancer() = default;

    void SAHKDTreeBalancer::testSplit(
        const KDTreeBuildParameters& parameters, const Float4& position, ASizeT axis,
        const PGeometryNodeList& geometry, const AxisAlignedBoundingBox& parentBounding, Float& bestCost,
        KDTreePlane& bestPlane
    ) const
    {
        const auto splitCoordOnAxis = position[axis];
        const auto splitPlane = sampleSplittingPlane(axis, parentBounding, position);

        AxisAlignedBoundingBox leftBounding, rightBounding;
        KDTreeBounding::split(splitPlane, parentBounding, leftBounding, rightBounding);

        auto leftGeometry = PGeometryNodeList();
        std::copy_if(geometry.cbegin(), geometry.cend(), std::back_inserter(leftGeometry), [=](auto geometryNode)
        {
            return SAHKDTreeBalancer::geometryNodeBounding(*geometryNode).minimum[axis] <= splitCoordOnAxis;
        });

        leftBounding = clamp(leftBounding, KDTreeBounding::findMinimumBoundingOfGeometry(leftGeometry));
        if (isTerminal(parameters, Zero<ASizeT>(), leftBounding, leftGeometry))
        {
            return;
        }

        auto rightGeometry = PGeometryNodeList();
        std::copy_if(geometry.cbegin(), geometry.cend(), std::back_inserter(rightGeometry), [=](auto geometryNode)
        {
            return SAHKDTreeBalancer::geometryNodeBounding(*geometryNode).maximum[axis] >= splitCoordOnAxis;
        });

        rightBounding = clamp(rightBounding, KDTreeBounding::findMinimumBoundingOfGeometry(rightGeometry));
        if (isTerminal(parameters, Zero<ASizeT>(), rightBounding, rightGeometry))
        {
            return;
        }

        const auto splitCost =
            calculateSplitCost(parameters, parentBounding, leftBounding, rightBounding, leftGeometry, rightGeometry);

        const auto isBetter = splitCost < bestCost;
        bestCost = select(isBetter, splitCost, bestCost);
        bestPlane = select(isBetter, splitPlane, bestPlane);
    }

    void SAHKDTreeBalancer::sweepFindBestSplit(
        const KDTreeBuildParameters& parameters, ASizeT axis, const PGeometryNodeList& geometry,
        const AxisAlignedBoundingBox& parentBounding, const std::vector<Float>& candidates, Float& bestCost,
        KDTreePlane& bestPlane
    ) const
    {
        const auto N = geometry.size();
        if (N == Zero<ASizeT>() || candidates.empty())
        {
            return;
        }

        const auto sortedByMin = geometrySortByMinimum(axis, geometry);
        const auto sortedByMax = geometrySortByMaximum(axis, geometry);

        std::vector<Float> minCoords(N);
        std::vector<AxisAlignedBoundingBox> prefixBounds(N);
        std::vector<Float> prefixCosts(N);
        {
            auto bb = AxisAlignedBoundingBox();
            auto costSum = Zero<Float>();
            for (auto i = Zero<ASizeT>(); i < N; ++i)
            {
                const auto nodeBox = geometryNodeBounding(*sortedByMin[i]);
                minCoords[i] = nodeBox.minimum[axis];
                bb = extendBy(bb, nodeBox);
                prefixBounds[i] = bb;
                costSum += sortedByMin[i]->getIndividualIntersectionCosts();
                prefixCosts[i] = costSum;
            }
        }

        std::vector<Float> maxCoords(N);
        std::vector<AxisAlignedBoundingBox> suffixBounds(N);
        std::vector<Float> suffixCosts(N);
        {
            auto bb = AxisAlignedBoundingBox();
            auto costSum = Zero<Float>();
            for (auto i = N; i > Zero<ASizeT>(); --i)
            {
                const auto nodeBox = geometryNodeBounding(*sortedByMax[i - 1]);
                maxCoords[i - 1] = nodeBox.maximum[axis];
                bb = extendBy(bb, nodeBox);
                suffixBounds[i - 1] = bb;
                costSum += sortedByMax[i - 1]->getIndividualIntersectionCosts();
                suffixCosts[i - 1] = costSum;
            }
        }

        const auto parentSA = surfaceArea(parentBounding);
        const auto intersectionCostFactor = y(parameters.costParameters) / parentSA;
        const auto traversalCost = x(parameters.costParameters);
        const auto parentOffset = parentBounding.minimum[axis];
        const auto parentNorm = extents(parentBounding)[axis];

        for (const auto splitCoord : candidates)
        {
            const auto leftIt = std::upper_bound(minCoords.cbegin(), minCoords.cend(), splitCoord);
            const auto leftCount = static_cast<ASizeT>(leftIt - minCoords.cbegin());

            const auto rightIt = std::lower_bound(maxCoords.cbegin(), maxCoords.cend(), splitCoord);
            const auto rightStart = static_cast<ASizeT>(rightIt - maxCoords.cbegin());
            const auto rightCount = N - rightStart;

            if (leftCount == Zero<ASizeT>() || rightCount == Zero<ASizeT>())
            {
                continue;
            }

            if (leftCount <= parameters.maxNodesSize || rightCount <= parameters.maxNodesSize)
            {
                continue;
            }

            const auto leftSplitBB = AxisAlignedBoundingBox(
                parentBounding.minimum, replaceComponent(parentBounding.maximum, splitCoord, axis)
            );
            const auto leftBounding = clamp(leftSplitBB, prefixBounds[leftCount - 1]);

            if (anyTrue3(extents(leftBounding) < Epsilon<Float4>()))
            {
                continue;
            }

            const auto rightSplitBB = AxisAlignedBoundingBox(
                replaceComponent(parentBounding.minimum, splitCoord, axis), parentBounding.maximum
            );
            const auto rightBounding = clamp(rightSplitBB, suffixBounds[rightStart]);

            if (anyTrue3(extents(rightBounding) < Epsilon<Float4>()))
            {
                continue;
            }

            const auto leftCosts = One<Float>() + prefixCosts[leftCount - 1];
            const auto rightCosts = One<Float>() + suffixCosts[rightStart];
            const auto splitCost =
                traversalCost + intersectionCostFactor * (surfaceArea(leftBounding) * leftCosts +
                    surfaceArea(rightBounding) * rightCosts);

            if (splitCost < bestCost)
            {
                bestCost = splitCost;
                bestPlane = sampleSplittingPlane(axis, parentOffset, parentNorm, splitCoord);
            }
        }
    }

    const AxisAlignedBoundingBox SAHKDTreeBalancer::geometryNodeBounding(const GeometryNode& node)
    {
        return node.includeInBounding(AxisAlignedBoundingBox());
    }
}
