#pragma once

#include "GeometryNode.h"
#include "KDTreeRoot.h"
#include "KDTreeBuildParameters.h"

namespace raytracer
{
    using namespace vectorization;
    using namespace primitives;

    class KDTreeBalancer
    {
        void build(
            const KDTreeBuildParameters & parameters,
            const ASizeT treeDepth,
            const KDTreePlane * const parentSplitter,
            const AxisAlignedBoundingBox & parentBounding,
            KDTreeNode & parentNode
        ) const;

        void sort(
            const KDTreePlane & splitter,
            const PGeometryNodeList & geometry,
            const AxisAlignedBoundingBox & leftBounding,
            const AxisAlignedBoundingBox & rightBounding,
            PGeometryNodeList *& leftGeometry,
            PGeometryNodeList *& rightGeometry
        ) const;

        virtual const KDTreePlane findSplitter(
            const KDTreeBuildParameters & paramaters,
            const ASizeT treeDepth,
            const AxisAlignedBoundingBox & bounding,
            const PGeometryNodeList & geometry,
            const KDTreePlane * parentSplitter
        ) const = 0;

    public:

        virtual ~KDTreeBalancer();

        virtual KDTreeRoot * const build(const PGeometryNodeList & rootGeometry) const;

        static const bool isTerminal(
            const KDTreeBuildParameters & parameters,
            const ASizeT treeDepth,
            const AxisAlignedBoundingBox & bounding,
            const PGeometryNodeList & geometry
        );

        static const bool isTerminalPostCheck(
            const KDTreeBuildParameters & parameters,
            const ASizeT treeDepth,
            const AxisAlignedBoundingBox & bounding,
            const PGeometryNodeList & geometry,
            const AxisAlignedBoundingBox & leftBounding,
            const AxisAlignedBoundingBox & rightBounding,
            const PGeometryNodeList & leftGeometry,
            const PGeometryNodeList & rightGeometry
        );

        static const Float calculateSplitCost(
            const KDTreeBuildParameters & parameters,
            const AxisAlignedBoundingBox & parentBounding,
            const AxisAlignedBoundingBox & leftBounding,
            const AxisAlignedBoundingBox & rightBounding,
            const PGeometryNodeList & leftGeometry,
            const PGeometryNodeList & rightGeometry
        );

        static const Float calculateLeafCost(const KDTreeBuildParameters & parameters, const PGeometryNodeList & geometry);
    };
}
