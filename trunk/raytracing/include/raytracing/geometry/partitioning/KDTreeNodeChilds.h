#pragma once

#include "KDTreeNode.h"

namespace raytracer
{
	using namespace primitives;

	// KD-trees have a very special structure: Non-leaf-nodes always have two
	// child nodes. That is why we use a dedicated KDTreeNodeChilds structure
	// that reflects that inherent property.
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
