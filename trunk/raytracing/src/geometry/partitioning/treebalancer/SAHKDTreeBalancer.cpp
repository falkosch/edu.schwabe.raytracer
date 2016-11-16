#include "../../../stdafx.h"

#include "geometry/partitioning/treebalancer/SAHKDTreeBalancer.h"

namespace raytracer
{

    SAHKDTreeBalancer::~SAHKDTreeBalancer() { }

    void SAHKDTreeBalancer::testSplit(
        const KDTreeBuildParameters & parameters,
        const Float4 & position,
        const ASizeT axis,
        const PGeometryNodeList & geometry,
        const KDTreeBounding & parentBounding,
        Float & bestCost,
        KDTreePlane & bestPlane) const
    {
        const Float splitCoordOnAxis = position[axis];
        const KDTreePlane splitPlane = SamplingKDTreeBalancer::sampleSplittingPlane(axis, parentBounding, position);

        KDTreeBounding leftBounding, rightBounding;
        parentBounding.split(splitPlane, leftBounding, rightBounding);

        PGeometryNodeList leftGeometry = SamplingKDTreeBalancer::geometrySortByMinimum(axis, geometry);
        for (PGeometryNodeList::const_iterator it = leftGeometry.cbegin(); it != leftGeometry.cend(); ++it)
        {
            if (SAHKDTreeBalancer::geometryNodeBounding(**it).minimum[axis] > splitCoordOnAxis)
            {
                leftGeometry = PGeometryNodeList(leftGeometry.cbegin(), it);
                break;
            }
        }
        leftBounding = clamp(leftBounding, KDTreeBounding::findMinimumBoundingOfGeometry(leftGeometry));
        if (isTerminal(parameters, Zero<ASizeT>(), leftBounding, leftGeometry))
        {
            return;
        }

        PGeometryNodeList rightGeometry = SamplingKDTreeBalancer::geometrySortByMaximum(axis, geometry);
        for (PGeometryNodeList::const_iterator it = rightGeometry.cbegin(); it != rightGeometry.cend(); ++it)
        {
            if (SAHKDTreeBalancer::geometryNodeBounding(**it).maximum[axis] >= splitCoordOnAxis)
            {
                rightGeometry = PGeometryNodeList(it, rightGeometry.cend());
                break;
            }
        }
        rightBounding = clamp(rightBounding, KDTreeBounding::findMinimumBoundingOfGeometry(rightGeometry));
        if (isTerminal(parameters, Zero<ASizeT>(), rightBounding, rightGeometry))
        {
            return;
        }

        const Float splitCost = calculateSplitCost(
            parameters,
            parentBounding,
            leftBounding,
            rightBounding,
            leftGeometry,
            rightGeometry);

        const bool isBetter = splitCost < bestCost;
        bestCost = select(isBetter, splitCost, bestCost);
        bestPlane = select(isBetter, splitPlane, bestPlane);
    }

    const AxisAlignedBoundingBox SAHKDTreeBalancer::geometryNodeBounding(const GeometryNode & node) const
    {
        AxisAlignedBoundingBox aabb = AxisAlignedBoundingBox();
        node.includeInBounding(aabb);
        return aabb;
    }

}
