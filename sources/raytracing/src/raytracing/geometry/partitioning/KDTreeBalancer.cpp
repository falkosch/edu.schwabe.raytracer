#include "raytracing/geometry/partitioning/KDTreeBalancer.h"
#include "../../../stdafx.h"

#include "raytracing/geometry/partitioning/KDTreeNodeChildren.h"

#include <array>
#include <cassert>
#include <intrin.h>
#include <limits>
#include <logging.h>
#include <numeric>
#include <omp.h>
#include <sstream>

static const auto Log = logging::scope("KDTree");

namespace raytracer
{
    namespace
    {
        // Copies the pointer-based build tree into the flat arrays of the root, depth-first, so that child A follows
        // its parent directly. Returns the link to the copied node.
        KDTreeLink flatten(const KDTreeNode& node, KDTreeRoot& root)
        {
            if (!node.children)
            {
                const auto offset = root.leafGeometry.size();
                const auto count = node.geometryNodes ? node.geometryNodes->size() : ASizeT{0};
                if (count > 0)
                {
                    root.leafGeometry.insert(
                        root.leafGeometry.end(), node.geometryNodes->cbegin(), node.geometryNodes->cend()
                    );
                }
                assert(root.leafGeometry.size() <= std::numeric_limits<std::uint32_t>::max());
                root.leaves.push_back(
                    KDTreeLeaf{static_cast<std::uint32_t>(offset), static_cast<std::uint32_t>(count)}
                );
                assert(root.leaves.size() <= KDTreeRoot::LeafFlag);
                return static_cast<KDTreeLink>(root.leaves.size() - 1) | KDTreeRoot::LeafFlag;
            }

            const auto inner = root.childLinks.size();
            assert(inner < KDTreeRoot::LeafFlag);
            root.childBoundings.push_back(KDTreeChildBoundings{node.children->boundingA, node.children->boundingB});
            root.childLinks.emplace_back();
            const auto childA = flatten(node.children->childA, root);
            const auto childB = flatten(node.children->childB, root);
            root.childLinks[inner] = KDTreeChildLinks{childA, childB};
            return static_cast<KDTreeLink>(inner);
        }

        // Builds the wide node for a binary inner node from its children's children, depth-first. Returns its index.
        KDTreeLink collapse(const KDTreeLink inner, KDTreeRoot& root)
        {
            std::array<AxisAlignedBoundingBox, 4> boxes;
            std::array<KDTreeLink, 4> links{};
            auto count = ASizeT{0};
            const auto addLanes = [&](const AxisAlignedBoundingBox& box, const KDTreeLink child)
            {
                if (KDTreeRoot::isLeaf(child))
                {
                    boxes[count] = box;
                    links[count++] = child;
                    return;
                }
                const auto& grandchildBoxes = root.childBoundings[child];
                const auto& grandchildren = root.childLinks[child];
                boxes[count] = grandchildBoxes.boundingA;
                links[count++] = grandchildren.childA;
                boxes[count] = grandchildBoxes.boundingB;
                links[count++] = grandchildren.childB;
            };
            addLanes(root.childBoundings[inner].boundingA, root.childLinks[inner].childA);
            addLanes(root.childBoundings[inner].boundingB, root.childLinks[inner].childB);

            const auto index = root.wideNodes.size();
            assert(index < KDTreeRoot::LeafFlag);
            root.wideNodes.emplace_back();
            for (auto lane = ASizeT{0}; lane < count; ++lane)
            {
                if (!KDTreeRoot::isLeaf(links[lane]))
                {
                    links[lane] = collapse(links[lane], root);
                }
            }

            // root.wideNodes may have grown in the meantime
            auto& node = root.wideNodes[index];
            node.minimumX = node.minimumY = node.minimumZ = node.maximumX = node.maximumY = node.maximumZ =
                Zero<Float4>();
            for (auto lane = ASizeT{0}; lane < count; ++lane)
            {
                node.minimumX[lane] = x(boxes[lane].minimum);
                node.minimumY[lane] = y(boxes[lane].minimum);
                node.minimumZ[lane] = z(boxes[lane].minimum);
                node.maximumX[lane] = x(boxes[lane].maximum);
                node.maximumY[lane] = y(boxes[lane].maximum);
                node.maximumZ[lane] = z(boxes[lane].maximum);
            }
            node.links = links;
            node.lanes = (1u << count) - 1u;
            return static_cast<KDTreeLink>(index);
        }
    }

    KDTreeBalancer::~KDTreeBalancer() = default;

    bool KDTreeBalancer::isTerminal(
        const KDTreeBuildParameters& parameters, const ASizeT treeDepth, const AxisAlignedBoundingBox& bounding,
        const PGeometryNodeList& geometry
    )
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

    bool KDTreeBalancer::isTerminalPostCheck(
        const KDTreeBuildParameters& parameters, const AxisAlignedBoundingBox& parentBounding,
        const PGeometryNodeList& geometry, const AxisAlignedBoundingBox& leftBounding,
        const AxisAlignedBoundingBox& rightBounding, const PGeometryNodeList& leftGeometry,
        const PGeometryNodeList& rightGeometry
    )
    {
        // Children small enough to be leaves don't prevent the split: maxNodesSize is the maximum leaf size.
        return calculateLeafCost(parameters, geometry)
            <= calculateSplitCost(parameters, parentBounding, leftBounding, rightBounding, leftGeometry, rightGeometry);
    }

    std::unique_ptr<KDTreeRoot> KDTreeBalancer::build(const PGeometryNodeList& rootGeometry) const
    {
        if (rootGeometry.empty())
        {
            return nullptr;
        }

        auto parameters = KDTreeBuildParameters();
        parameters.costParameters = Float4(4.68375f, 1.0f);
        parameters.maxNodesSize = ASizeT{16};
        parameters.maxTreeDepth =
            ASizeT{1} + static_cast<ASizeT>(1.1f * logN(static_cast<Float>(rootGeometry.size() + ASizeT{1}), 2.0f));

        auto root = std::make_unique<KDTreeRoot>();
        root->rootBounding = KDTreeBounding::findMinimumBoundingOfGeometry(rootGeometry);

        const auto cycles0 = __rdtsc();

        // build with heap-allocated nodes first (the final size is unknown), then copy into the flat arrays
        auto buildRoot = KDTreeNode();
        buildRoot.geometryNodes = std::make_unique<PGeometryNodeList>(rootGeometry);
        build(parameters, Zero<ASizeT>(), nullptr, root->rootBounding, buildRoot);

        root->treeDepth = buildRoot.depth();
        assert(root->treeDepth <= KDTreeRoot::MaxDepth);
        root->childBoundings.reserve(buildRoot.nodeCount() / 2);
        root->childLinks.reserve(buildRoot.nodeCount() / 2);
        root->leaves.reserve(buildRoot.nodeCount() / 2 + 1);
        root->rootLink = flatten(buildRoot, *root);
        root->wideNodes.reserve(root->childLinks.size() / 2 + 1);
        root->wideRootLink = KDTreeRoot::isLeaf(root->rootLink) ? root->rootLink : collapse(root->rootLink, *root);

        const auto cycles1 = __rdtsc();
        Log.info([n = rootGeometry.size(), c = cycles1 - cycles0]
        {
            return "Built KD-tree for " + std::to_string(n) + " nodes in " + std::to_string(c) + " cycles";
        });

        return root;
    }

    void KDTreeBalancer::build(
        const KDTreeBuildParameters& parameters, const ASizeT treeDepth, const KDTreePlane* const parentSplitter,
        const AxisAlignedBoundingBox& parentBounding, KDTreeNode& parentNode
    ) const
    {
        KDTreePlane splitter;
        AxisAlignedBoundingBox leftBounding, rightBounding;
        std::unique_ptr<PGeometryNodeList> leftGeometry, rightGeometry;
        const PGeometryNodeList& parentGeometry = *parentNode.geometryNodes;
        assert(!parentGeometry.empty());

        // check termination-conditions by balancer
        if (isTerminal(parameters, treeDepth, parentBounding, parentGeometry))
        {
            return;
        }

        // let the balancer find a split-plane
        splitter = findSplitter(parameters, treeDepth, parentBounding, parentGeometry, parentSplitter);
        KDTreeBounding::split(splitter, parentBounding, leftBounding, rightBounding);

        // sort geometry to left and right bounding
        sort(splitter, parentGeometry, leftBounding, rightBounding, leftGeometry, rightGeometry);

        // optimization: shrink bounding boxes to primitives and clamp them to the original sizes
        leftBounding = clamp(leftBounding, KDTreeBounding::findMinimumBoundingOfGeometry(*leftGeometry));
        rightBounding = clamp(rightBounding, KDTreeBounding::findMinimumBoundingOfGeometry(*rightGeometry));

        // post-check the termination-conditions again
        const ASizeT treeDepthNext = treeDepth + One<ASizeT>();
        if (isTerminalPostCheck(
            parameters, parentBounding, parentGeometry, leftBounding, rightBounding, *leftGeometry, *rightGeometry
        ))
        {
            return;
        }

        // grow tree by new children
        parentNode.grow(leftBounding, std::move(leftGeometry), rightBounding, std::move(rightGeometry));

        // recurse into new left and right child nodes
        build(parameters, treeDepthNext, &splitter, leftBounding, parentNode.children->childA);
        build(parameters, treeDepthNext, &splitter, rightBounding, parentNode.children->childB);
    }

    void KDTreeBalancer::sort(
        const KDTreePlane&, const PGeometryNodeList& geometry, const AxisAlignedBoundingBox& leftBounding,
        const AxisAlignedBoundingBox& rightBounding, std::unique_ptr<PGeometryNodeList>& leftGeometry,
        std::unique_ptr<PGeometryNodeList>& rightGeometry
    ) const
    {
        const int geometrySize = static_cast<int>(geometry.size());
        leftGeometry = std::make_unique<PGeometryNodeList>();
        rightGeometry = std::make_unique<PGeometryNodeList>();

        const auto numThreads = omp_get_max_threads();
        std::vector<PGeometryNodeList> allLeftGeometry(numThreads);
        std::vector<PGeometryNodeList> allRightGeometry(numThreads);

#pragma omp parallel if (geometrySize >= 64)
        {
            auto& tLeftGeometry = allLeftGeometry[omp_get_thread_num()];
            tLeftGeometry.reserve(geometry.size() / numThreads + 1);
            auto& tRightGeometry = allRightGeometry[omp_get_thread_num()];
            tRightGeometry.reserve(geometry.size() / numThreads + 1);

#pragma omp for nowait
            for (int i = Zero<int>(); i < geometrySize; ++i)
            {
                GeometryNode* const geometryNode = geometry[static_cast<ASizeT>(i)];
                bool inserted = false;

                // Ask the geometryNode to test for an overlap with one of the two bounds
                if (geometryNode->overlaps(leftBounding))
                {
                    tLeftGeometry.push_back(geometryNode);
                    inserted |= true;
                }
                if (geometryNode->overlaps(rightBounding))
                {
                    tRightGeometry.push_back(geometryNode);
                    inserted |= true;
                }

                // For overlap-test-algorithms, which are not robust enough, we fall back to
                // simple AABB overlap tests to not lose any geometry due to falsy
                // overlap-test results. That is of course not optimal and causes an
                // unnecessarily deep tree due to more splits. Make sure the tree depth is
                // finite or use the SAH balancers, which will early stop too many splits
                // when traversal costs exceed the intersection costs.
                if (!inserted)
                {
                    const AxisAlignedBoundingBox aabb = geometryNode->includeInBounding(AxisAlignedBoundingBox());
                    if (overlaps(aabb, leftBounding))
                    {
                        tLeftGeometry.push_back(geometryNode);
                        inserted |= true;
                    }
                    if (overlaps(aabb, rightBounding))
                    {
                        tRightGeometry.push_back(geometryNode);
                        inserted |= true;
                    }
                }

                assert(inserted);
            }
        }

        // merge thread-local buffers without locks
        ASizeT totalLeft{0}, totalRight{0};
        for (auto t = 0; t < numThreads; t++)
        {
            totalLeft += allLeftGeometry[t].size();
            totalRight += allRightGeometry[t].size();
        }
        leftGeometry->reserve(totalLeft);
        rightGeometry->reserve(totalRight);
        for (auto t = 0; t < numThreads; t++)
        {
            leftGeometry->insert(leftGeometry->end(), allLeftGeometry[t].cbegin(), allLeftGeometry[t].cend());
            rightGeometry->insert(rightGeometry->end(), allRightGeometry[t].cbegin(), allRightGeometry[t].cend());
        }
    }

    Float KDTreeBalancer::calculateSplitCost(
        const KDTreeBuildParameters& parameters, const AxisAlignedBoundingBox& parentBounding,
        const AxisAlignedBoundingBox& leftBounding, const AxisAlignedBoundingBox& rightBounding,
        const PGeometryNodeList& leftGeometry, const PGeometryNodeList& rightGeometry
    )
    {
        // based on: Wald and Havran, "On building fast kd-Trees for Ray Tracing, and on doing that in O(N log N)", 2006
        // x = number of geometry nodes in left bounding
        // y = ... right bounding
        // z = w = 0
        // const Float4 TleftAndRight = convert<Float4>(Size2(leftGeometry.size(), rightGeometry.size()));

        // alternative uses the actual summed up intersection costs of each individual
        //
        // note that the following loops cannot be reduced to the expression: xCosts = x.size() *
        // x[0]->getIndividualIntersectionCosts() because getIndividualIntersectionCosts() may not be constant over all
        // entries in the geometryNodeLists, f.e. for the scene object lists
        const Float leftCosts = std::accumulate(
            leftGeometry.cbegin(), leftGeometry.cend(), One<Float>(),
            [](const Float accLeftCosts, auto geometryNode)
            {
                return accLeftCosts + geometryNode->getIndividualIntersectionCosts();
            }
        );
        const Float rightCosts = std::accumulate(
            rightGeometry.cbegin(), rightGeometry.cend(), One<Float>(),
            [](const Float accRightCosts, auto geometryNode)
            {
                return accRightCosts + geometryNode->getIndividualIntersectionCosts();
            }
        );
        const auto TleftAndRight = Float4(leftCosts, rightCosts);

        // x = surface area of left bounding after split
        // y = ... right bounding ...
        // z = w = 0
        const auto SAleftAndRight = Float4(surfaceArea(leftBounding), surfaceArea(rightBounding));

        // post-factorization of the intersection costs
        const Float4 intersectionCostFactor = yyyy(parameters.costParameters / surfaceArea(parentBounding));

        // overall costs for testing left and right bounding
        const Float4 testingLeavesCost = intersectionCostFactor * SAleftAndRight * TleftAndRight;
        assert(allTrue(xyxy(testingLeavesCost) > Zero<Float4>()));

        // summed up costs for splitting parent bounding into given left and right bounding with given left and right
        // geometries
        const Float splitCost = x(parameters.costParameters + testingLeavesCost + yyyy(testingLeavesCost));
        assert(splitCost > Zero<Float>());

        return splitCost;
    }

    Float
    KDTreeBalancer::calculateLeafCost(const KDTreeBuildParameters& parameters, const PGeometryNodeList& geometry)
    {
        const Float costs =
            std::accumulate(geometry.cbegin(), geometry.cend(), One<Float>(),
                            [](const Float accCosts, auto geometryNode)
                            {
                                return accCosts + geometryNode->getIndividualIntersectionCosts();
                            });
        return y(parameters.costParameters) * costs;
    }
}
