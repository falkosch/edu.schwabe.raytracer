#include "../../stdafx.h"

#include "geometry/partitioning/KDTreeNodeChilds.h"

namespace raytracer
{

	KDTreeNodeChilds::KDTreeNodeChilds()
		:
		boundingA(),
		boundingB(),
		childA(),
		childB()
	{ }

	KDTreeNodeChilds::~KDTreeNodeChilds() { }

}
