#pragma once

#include "geometry/partitioning/KDTreeNodeChilds.h"

namespace raytracer
{

    struct KDTreeRoot
    {
        KDTreeBounding rootBounding;

        KDTreeNode rootNode;

        ALIGNED_ALLOCATORS(__alignof(KDTreeRoot));

        KDTreeRoot();

        ~KDTreeRoot();
    };

}
