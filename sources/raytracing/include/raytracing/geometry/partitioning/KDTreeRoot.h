#pragma once

#include "GeometryNode.h"
#include "KDTreeChildBoundings.h"
#include "KDTreeChildLinks.h"
#include "KDTreeLeaf.h"
#include "KDTreeLink.h"
#include "KDTreeWideNode.h"

#include <vector>

namespace raytracer
{
    using namespace primitives;

    // KD-tree in flat arrays. Inner nodes are stored depth-first (child A right after its parent), children and leaves
    // are referenced by index. Built by KDTreeBalancer::build().
    struct KDTreeRoot
    {
        static constexpr KDTreeLink LeafFlag = KDTreeLink{1} << 31;

        // upper bound for depth(), sizes the traversal stacks
        static constexpr ASizeT MaxDepth = 64;

        AxisAlignedBoundingBox rootBounding;

        // inner node 0, or a leaf if the whole tree is a single leaf
        KDTreeLink rootLink;

        // per inner node, same index in both
        std::vector<KDTreeChildBoundings> childBoundings;
        std::vector<KDTreeChildLinks> childLinks;

        std::vector<KDTreeLeaf> leaves;

        // the geometry of all leaves back to back; geometry straddling a split occurs once per leaf
        PGeometryNodeList leafGeometry;

        // longest path from the root to a leaf, counted in nodes
        ASizeT treeDepth;

        // the same tree with up to KDTreeWideNode::Width children per node, built from the arrays above by
        // buildWideNodes(); wideRootLink is a leaf link if the whole tree is a single leaf
        KDTreeLink wideRootLink;
        std::vector<KDTreeWideNode> wideNodes;

        KDTreeRoot();

        ~KDTreeRoot();

        static bool isLeaf(KDTreeLink link) noexcept;

        // (Re)builds wideRootLink and wideNodes from rootLink and the inner node arrays.
        void buildWideNodes();

        // geometry of the leaf the link points to
        GeometryNodeSpan geometryOf(KDTreeLink leafLink) const noexcept;

        ASizeT depth() const;

        // inner nodes plus leaves
        ASizeT nodeCount() const;
    };
}
