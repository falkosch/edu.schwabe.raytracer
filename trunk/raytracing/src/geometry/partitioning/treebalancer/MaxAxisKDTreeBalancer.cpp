#include "../../../stdafx.h"

#include "geometry/partitioning/treebalancer/MaxAxisKDTreeBalancer.h"

namespace raytracer
{

	MaxAxisKDTreeBalancer::MaxAxisKDTreeBalancer() : KDTreeBalancer() { }

	MaxAxisKDTreeBalancer::~MaxAxisKDTreeBalancer() { }

	const KDTreePlane MaxAxisKDTreeBalancer::findSplitter(
		const KDTreeBuildParameters &,
		const ASizeT,
		const AxisAlignedBoundingBox & bounding,
		const PGeometryNodeList &,
		const KDTreePlane *
	) const
	{
		return KDTreePlane(Half<Float>(), argmax3(extents(bounding)));
	}

}
