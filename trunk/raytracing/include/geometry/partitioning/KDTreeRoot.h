#pragma once

#include "geometry/partitioning/KDTreeNodeChilds.h"

namespace raytracer
{

	struct KDTreeRoot
	{
		AxisAlignedBoundingBox rootBounding;

		KDTreeNode rootNode;

		ALIGNED_ALLOCATORS(__alignof(KDTreeRoot));

		KDTreeRoot();

		~KDTreeRoot();
	};

}
