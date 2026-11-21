#include "raytracing/geometry/partitioning/KDTreeRoot.h"
#include "../../../stdafx.h"

#include <array>
#include <cassert>

namespace raytracer
{
    namespace
    {
        using WideBoxes = std::array<AxisAlignedBoundingBox, KDTreeWideNode::Width>;
        using WideLinks = std::array<KDTreeLink, KDTreeWideNode::Width>;

        // Appends the lanes for child: the child itself if it is a leaf or levels is 0, otherwise the lanes of its
        // children one level further down, child A's first (depth-first, like the binary traversal).
        void addLanes(
            const KDTreeRoot& root, const AxisAlignedBoundingBox& box, const KDTreeLink child, const ASizeT levels,
            WideBoxes& boxes, WideLinks& links, ASizeT& count
        )
        {
            if (levels == Zero<ASizeT>() || KDTreeRoot::isLeaf(child))
            {
                boxes[count] = box;
                links[count++] = child;
                return;
            }
            const auto& childBoxes = root.childBoundings[child];
            const auto& grandchildren = root.childLinks[child];
            addLanes(root, childBoxes.boundingA, grandchildren.childA, levels - One<ASizeT>(), boxes, links, count);
            addLanes(root, childBoxes.boundingB, grandchildren.childB, levels - One<ASizeT>(), boxes, links, count);
        }

        // Builds the wide node for a binary inner node from its descendants KDTreeWideNode::Levels levels down,
        // depth-first. Returns its index.
        KDTreeLink collapse(const KDTreeLink inner, KDTreeRoot& root)
        {
            WideBoxes boxes;
            WideLinks links{};
            auto count = Zero<ASizeT>();
            addLanes(root, root.childBoundings[inner].boundingA, root.childLinks[inner].childA,
                     KDTreeWideNode::Levels - One<ASizeT>(), boxes, links, count);
            addLanes(root, root.childBoundings[inner].boundingB, root.childLinks[inner].childB,
                     KDTreeWideNode::Levels - One<ASizeT>(), boxes, links, count);

            const auto index = root.wideNodes.size();
            assert(index < KDTreeRoot::LeafFlag);
            root.wideNodes.emplace_back();
            for (auto lane = Zero<ASizeT>(); lane < count; ++lane)
            {
                if (!KDTreeRoot::isLeaf(links[lane]))
                {
                    links[lane] = collapse(links[lane], root);
                }
            }

            // root.wideNodes may have grown in the meantime
            auto& node = root.wideNodes[index];
            node.minimumX = node.minimumY = node.minimumZ = node.maximumX = node.maximumY = node.maximumZ =
                Zero<KDTreeWideLanes>();
            for (auto lane = Zero<ASizeT>(); lane < count; ++lane)
            {
                node.minimumX[lane] = x(boxes[lane].minimum);
                node.minimumY[lane] = y(boxes[lane].minimum);
                node.minimumZ[lane] = z(boxes[lane].minimum);
                node.maximumX[lane] = x(boxes[lane].maximum);
                node.maximumY[lane] = y(boxes[lane].maximum);
                node.maximumZ[lane] = z(boxes[lane].maximum);
            }
            node.links = links;
            node.lanes = (One<std::uint32_t>() << count) - One<std::uint32_t>();
            return static_cast<KDTreeLink>(index);
        }
    }

    KDTreeRoot::KDTreeRoot()
        : rootBounding(), rootLink(LeafFlag), childBoundings(), childLinks(), leaves(), leafGeometry(), treeDepth(),
          wideRootLink(LeafFlag), wideNodes()
    {
    }

    KDTreeRoot::~KDTreeRoot() = default;

    bool KDTreeRoot::isLeaf(const KDTreeLink link) noexcept
    {
        return (link & LeafFlag) != 0;
    }

    void KDTreeRoot::buildWideNodes()
    {
        wideNodes.clear();
        wideNodes.reserve(childLinks.size() / KDTreeWideNode::Levels + One<ASizeT>());
        wideRootLink = isLeaf(rootLink) ? rootLink : collapse(rootLink, *this);
        // the reserve above is an estimate
        wideNodes.shrink_to_fit();
    }

    GeometryNodeSpan KDTreeRoot::geometryOf(const KDTreeLink leafLink) const noexcept
    {
        assert(isLeaf(leafLink));
        const auto& leaf = leaves[leafLink & ~LeafFlag];
        return GeometryNodeSpan(leafGeometry.data() + leaf.offset, leaf.count);
    }

    ASizeT KDTreeRoot::depth() const
    {
        return treeDepth;
    }

    ASizeT KDTreeRoot::nodeCount() const
    {
        return childLinks.size() + leaves.size();
    }
}
