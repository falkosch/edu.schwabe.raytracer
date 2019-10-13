#pragma once

#include "KDTreeNodeChilds.h"

namespace raytracer
{
    using namespace primitives;

    struct KDTreeRoot
    {
        AxisAlignedBoundingBox rootBounding;

        KDTreeNode rootNode;

        KDTreeRoot();

        ~KDTreeRoot();
    };
}
