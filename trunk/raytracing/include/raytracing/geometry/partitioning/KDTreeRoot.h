#pragma once

#include "KDTreeNodeChilds.h"

namespace raytracer
{
	using namespace primitives;

	struct KDTreeRoot
	{
		AxisAlignedBoundingBox rootBounding;

		KDTreeNode rootNode;

		ALIGNED_ALLOCATORS(__alignof(KDTreeRoot));

		KDTreeRoot();

		~KDTreeRoot();
	};

}
