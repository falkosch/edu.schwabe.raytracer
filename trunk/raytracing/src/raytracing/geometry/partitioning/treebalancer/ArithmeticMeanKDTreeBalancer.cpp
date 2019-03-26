#include "raytracing/geometry/partitioning/treebalancer/ArithmeticMeanKDTreeBalancer.h"
#include "../../../../stdafx.h"

#include <numeric>

namespace raytracer
{

	ArithmeticMeanKDTreeBalancer::ArithmeticMeanKDTreeBalancer() : SamplingKDTreeBalancer() { }

	ArithmeticMeanKDTreeBalancer::~ArithmeticMeanKDTreeBalancer() { }

	const KDTreePlane ArithmeticMeanKDTreeBalancer::findSplitter(
		const KDTreeBuildParameters &,
		const ASizeT,
		const AxisAlignedBoundingBox & bounding,
		const PGeometryNodeList & geometry,
		const KDTreePlane *
	) const
	{
		const Float4 boundingExtents = extents(bounding);
		const ASizeT maxAxis = argmax3(boundingExtents);

		const Float sum = std::accumulate(geometry.cbegin(), geometry.cend(), Zero<Float>(), [=](const Float accSum, auto geometryNode) {
			return accSum + SamplingKDTreeBalancer::geometryNodeMaximumPredicate(*geometryNode)[maxAxis];
		});

		return SamplingKDTreeBalancer::sampleSplittingPlane(
			maxAxis,
			bounding.minimum,
			boundingExtents,
			Float4(sum / static_cast<Float>(geometry.size()))
		);
	}

}
