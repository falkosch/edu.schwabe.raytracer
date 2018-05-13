#include "../../stdafx.h"

#include "geometry/partitioning/KDTreeBalancer.h"

#include <thread>

//#define CHECK_FOR_LOST_GEOMETRY

namespace raytracer
{

    KDTreeBalancer::~KDTreeBalancer() { }

    const bool KDTreeBalancer::isTerminal(
        const KDTreeBuildParameters & parameters,
        const ASizeT treeDepth,
        const KDTreeBounding & bounding,
        const PGeometryNodeList & geometry)
    {
        if (treeDepth >= parameters.maxTreeDepth)
        {
            return true;
        }
        if (geometry.size() <= parameters.maxNodesSize)
        {
            return true;
        }
        return anyTrue3(extents(bounding) < Epsilon<Float4>());
    }

    const bool KDTreeBalancer::isTerminalPostCheck(
        const KDTreeBuildParameters & parameters,
        const ASizeT treeDepth,
        const KDTreeBounding & parentBounding,
        const PGeometryNodeList & geometry,
        const KDTreeBounding & leftBounding,
        const KDTreeBounding & rightBounding,
        const PGeometryNodeList & leftGeometry,
        const PGeometryNodeList & rightGeometry)
    {
        if (isTerminal(parameters, treeDepth, leftBounding, leftGeometry))
        {
            return true;
        }
        if (isTerminal(parameters, treeDepth, rightBounding, rightGeometry))
        {
            return true;
        }
        return calculateLeafCost(parameters, geometry) <= calculateSplitCost(
            parameters,
            parentBounding,
            leftBounding,
            rightBounding,
            leftGeometry,
            rightGeometry);
    }

    KDTreeRoot * const KDTreeBalancer::build(const PGeometryNodeList & rootGeometry) const
    {
        if (!rootGeometry.size())
        {
            return nullptr;
        }

        KDTreeBuildParameters parameters = KDTreeBuildParameters();
        parameters.costParameters = Float4(4.68375f, 1.f);
        parameters.maxNodesSize = static_cast<ASizeT>(4);
        parameters.maxTreeDepth = One<ASizeT>() + static_cast<ASizeT>(1.1f * logN(static_cast<Float>(rootGeometry.size() + One<ASizeT>()), Two<Float>()));

        KDTreeRoot * const root = new KDTreeRoot();
        root->rootBounding = KDTreeBounding::findMinimumBoundingOfGeometry(rootGeometry);
        root->rootNode.geometryNodes = new PGeometryNodeList(rootGeometry);

        build(parameters, Zero<ASizeT>(), nullptr, root->rootBounding, root->rootNode);

        return root;
    }

    void KDTreeBalancer::build(
        const KDTreeBuildParameters & parameters,
        const ASizeT treeDepth,
        const KDTreePlane * const parentSplitter,
        const KDTreeBounding & parentBounding,
        KDTreeNode & parentNode) const
    {
        KDTreePlane splitter;
        KDTreeBounding leftBounding, rightBounding;
        PGeometryNodeList *leftGeometry, *rightGeometry;
        const PGeometryNodeList & parentGeometry = *parentNode.geometryNodes;
        assert(!parentGeometry.empty());

        // check termination-conditions by balancer
        if (isTerminal(parameters, treeDepth, parentBounding, parentGeometry)) return;

        // let the balancer find a split-plane
        splitter = findSplitter(parameters, treeDepth, parentBounding, parentGeometry, parentSplitter);
        parentBounding.split(splitter, leftBounding, rightBounding);

        // sort geometry to left and right boundings
        sort(splitter, parentGeometry, leftBounding, rightBounding, leftGeometry, rightGeometry);

        // optimization: shrink bounding boxes to primitives and clamp them to the original sizes
        leftBounding = clamp(leftBounding, KDTreeBounding::findMinimumBoundingOfGeometry(*leftGeometry));
        rightBounding = clamp(rightBounding, KDTreeBounding::findMinimumBoundingOfGeometry(*rightGeometry));

        // post-check the termination-conditions again
        const ASizeT treeDepthNext = treeDepth + One<ASizeT>();
        if (isTerminalPostCheck(
            parameters,
            treeDepthNext,
            parentBounding,
            parentGeometry,
            leftBounding,
            rightBounding,
            *leftGeometry,
            *rightGeometry))
        {
            // Remove left- and right-childs-lists, because the parent geometry will be left as is instead.
            delete leftGeometry;
            delete rightGeometry;
            return;
        }

#ifdef CHECK_FOR_LOST_GEOMETRY
        // check validity of split
        ASizeT lostCount = Zero<ASizeT>();
        for (PGeometryNodeList::const_iterator it = parentGeometry.cbegin(); it != parentGeometry.cend(); ++it)
        {
            lostCount += (std::find(leftGeometry->cbegin(), leftGeometry->cend(), *it) == leftGeometry->cend())
                && (std::find(rightGeometry->cbegin(), rightGeometry->cend(), *it) == rightGeometry->cend());
        }
        assert(!lostCount);
#endif

        // grow tree by new childs
        parentNode.grow(leftBounding, *leftGeometry, rightBounding, *rightGeometry);

        // recurse into new left and right child nodes
        build(parameters, treeDepthNext, &splitter, leftBounding, parentNode.childs->childA);
        build(parameters, treeDepthNext, &splitter, rightBounding, parentNode.childs->childB);
    }

    void KDTreeBalancer::sort(
        const KDTreePlane &,
        const PGeometryNodeList & geometry,
        const KDTreeBounding & leftBounding,
        const KDTreeBounding & rightBounding,
        PGeometryNodeList * & leftGeometry,
        PGeometryNodeList * & rightGeometry) const
    {
        const int geometrySize = static_cast<int>(geometry.size());
        leftGeometry = new PGeometryNodeList();
        rightGeometry = new PGeometryNodeList();

#pragma omp parallel if (geometrySize >= int(std::thread::hardware_concurrency()))
        {
            PGeometryNodeList * const tLeftGeometry = new PGeometryNodeList();
            tLeftGeometry->reserve(geometry.size());
            PGeometryNodeList * const tRightGeometry = new PGeometryNodeList();
            tRightGeometry->reserve(geometry.size());

#pragma omp for nowait
            for (int i = Zero<int>(); i < geometrySize; ++i)
            {
                GeometryNode * const geometryNode = geometry[static_cast<ASizeT>(i)];
#ifdef CHECK_FOR_LOST_GEOMETRY
                bool inserted = false;
                if (geometryNode->overlaps(leftBounding))
                {
                    tLeftGeometry->push_back(geometryNode);
                    inserted |= true;
                }
                if (geometryNode->overlaps(rightBounding))
                {
                    tRightGeometry->push_back(geometryNode);
                    inserted |= true;
                }
                assert(inserted);
#else
                if (geometryNode->overlaps(leftBounding))
                    tLeftGeometry->push_back(geometryNode);
                if (geometryNode->overlaps(rightBounding))
                    tRightGeometry->push_back(geometryNode);
#endif
            }

#pragma omp critical (appendLeftGeometry)
            {
                leftGeometry->insert(leftGeometry->end(), tLeftGeometry->cbegin(), tLeftGeometry->cend());
            }
#pragma omp critical (appendRightGeometry)
            {
                rightGeometry->insert(rightGeometry->end(), tRightGeometry->cbegin(), tRightGeometry->cend());
            }

            delete tLeftGeometry;
            delete tRightGeometry;
        }
    }

    const Float KDTreeBalancer::calculateSplitCost(
        const KDTreeBuildParameters & parameters,
        const AxisAlignedBoundingBox & parentBounding,
        const AxisAlignedBoundingBox & leftBounding,
        const AxisAlignedBoundingBox & rightBounding,
        const PGeometryNodeList & leftGeometry,
        const PGeometryNodeList & rightGeometry)
    {
        // based on: Wald and Havran, "On building fast kd-Trees for Ray Tracing, and on doing that in O(N log N)", 2006

        // calculate intersection costs by every individual
        Float leftCosts = One<Float>(), rightCosts = One<Float>();
        for (PGeometryNodeList::const_iterator it = leftGeometry.cbegin(); it != leftGeometry.cend(); ++it)
        {
            leftCosts += (*it)->getIndividualIntersectionCosts();
        }
        for (PGeometryNodeList::const_iterator it = rightGeometry.cbegin(); it != rightGeometry.cend(); ++it)
        {
            rightCosts += (*it)->getIndividualIntersectionCosts();
        }

        // x = number of geometry nodes in left bounding
        // y = ... right bounding
        // z = w = 0
        //const Float4 TleftAndRight = convert<Float4>(Size2(leftGeometry.size(), rightGeometry.size()));
        const Float4 TleftAndRight = Float4(leftCosts, rightCosts);

        // x = surface area of left bounding after split
        // y = ... right bounding ...
        // z = w = 0
        const Float4 SAleftAndRight = Float4(surfaceArea(leftBounding), surfaceArea(rightBounding));

        // post-factorization of the intersection costs
        const Float4 intersectionCostFactor = yyyy(parameters.costParameters / surfaceArea(parentBounding));

        // overall costs for testing left and right boundings
        const Float4 testingLeavesCost = intersectionCostFactor * SAleftAndRight * TleftAndRight;
        assert(allTrue(xyxy(testingLeavesCost) > Zero<Float4>()));

        // summed up costs for splitting parent bounding into given left and right boundings with given left and right geometries
        const Float splitCost = x(parameters.costParameters + testingLeavesCost + yyyy(testingLeavesCost));
        assert(splitCost > Zero<Float>());

        return splitCost;
    }

    const Float KDTreeBalancer::calculateLeafCost(
        const KDTreeBuildParameters & parameters,
        const PGeometryNodeList & geometry)
    {
        Float costs = One<Float>();
        for (PGeometryNodeList::const_iterator it = geometry.cbegin(); it != geometry.cend(); ++it)
        {
            costs += (*it)->getIndividualIntersectionCosts();
        }
        return y(parameters.costParameters) * costs;
    }

}
