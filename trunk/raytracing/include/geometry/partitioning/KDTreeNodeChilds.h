#pragma once

#include "geometry/partitioning/KDTreeNode.h"

namespace raytracer
{

    struct KDTreeNodeChilds
    {

		AxisAlignedBoundingBox boundingA, boundingB;

        // the two childs
        KDTreeNode childA, childB;

        ALIGNED_ALLOCATORS(__alignof(KDTreeNodeChilds));

        KDTreeNodeChilds();

        ~KDTreeNodeChilds();

    };

}
