#include "raytracing/geometry/partitioning/treebalancer/RotatingAxisKDTreeBalancer.h"
#include "../../../../stdafx.h"

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
