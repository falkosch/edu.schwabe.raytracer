#pragma once

#include "GeometryNode.h"
#include "KDTreeBuildParameters.h"
#include "KDTreeRoot.h"

#include <memory>

namespace raytracer
{
    using namespace vectorization;
    using namespace primitives;

    class KDTreeBalancer
    {
        void build(
            const KDTreeBuildParameters& parameters, ASizeT treeDepth, const KDTreePlane* parentSplitter,
            const AxisAlignedBoundingBox& parentBounding, KDTreeNode& parentNode
        ) const;

        void sort(
            const KDTreePlane& splitter, const PGeometryNodeList& geometry, const AxisAlignedBoundingBox& leftBounding,
            const AxisAlignedBoundingBox& rightBounding, std::unique_ptr<PGeometryNodeList>& leftGeometry,
            std::unique_ptr<PGeometryNodeList>& rightGeometry
        ) const;

        virtual const KDTreePlane findSplitter(
            const KDTreeBuildParameters& parameters, ASizeT treeDepth, const AxisAlignedBoundingBox& bounding,
            const PGeometryNodeList& geometry, const KDTreePlane* parentSplitter
        ) const = 0;

    public:
        virtual ~KDTreeBalancer();

        virtual std::unique_ptr<KDTreeRoot> build(const PGeometryNodeList& rootGeometry) const;

        static bool isTerminal(
            const KDTreeBuildParameters& parameters, ASizeT treeDepth, const AxisAlignedBoundingBox& bounding,
            const PGeometryNodeList& geometry
        );

        static bool isTerminalPostCheck(
            const KDTreeBuildParameters& parameters, ASizeT treeDepth, const AxisAlignedBoundingBox& bounding,
            const PGeometryNodeList& geometry, const AxisAlignedBoundingBox& leftBounding,
            const AxisAlignedBoundingBox& rightBounding, const PGeometryNodeList& leftGeometry,
            const PGeometryNodeList& rightGeometry
        );

        static Float calculateSplitCost(
            const KDTreeBuildParameters& parameters, const AxisAlignedBoundingBox& parentBounding,
            const AxisAlignedBoundingBox& leftBounding, const AxisAlignedBoundingBox& rightBounding,
            const PGeometryNodeList& leftGeometry, const PGeometryNodeList& rightGeometry
        );

        static Float calculateLeafCost(const KDTreeBuildParameters& parameters, const PGeometryNodeList& geometry);
    };
}
