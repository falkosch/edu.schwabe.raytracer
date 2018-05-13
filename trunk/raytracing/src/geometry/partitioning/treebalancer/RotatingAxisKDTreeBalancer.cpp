#include "../../../stdafx.h"

#include "geometry/partitioning/treebalancer/RotatingAxisKDTreeBalancer.h"

namespace raytracer
{

	RotatingAxisKDTreeBalancer::RotatingAxisKDTreeBalancer() : KDTreeBalancer() { }

	RotatingAxisKDTreeBalancer::~RotatingAxisKDTreeBalancer() { }

	const KDTreePlane RotatingAxisKDTreeBalancer::findSplitter(
		const KDTreeBuildParameters &,
		const ASizeT treeDepth,
		const AxisAlignedBoundingBox &,
		const PGeometryNodeList &,
		const KDTreePlane *
	) const
	{
		return KDTreePlane(Half<Float>(), treeDepth % D);
	}

}
