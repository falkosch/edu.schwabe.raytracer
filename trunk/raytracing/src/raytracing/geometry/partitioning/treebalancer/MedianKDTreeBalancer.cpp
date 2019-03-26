#include "raytracing/geometry/partitioning/treebalancer/MedianKDTreeBalancer.h"
#include "../../../../stdafx.h"

#include <cassert>

namespace raytracer
{

	MedianKDTreeBalancer::MedianKDTreeBalancer() : SamplingKDTreeBalancer() { }

	MedianKDTreeBalancer::~MedianKDTreeBalancer() { }

	const KDTreePlane MedianKDTreeBalancer::findSplitter(
		const KDTreeBuildParameters &,
		const ASizeT,
		const AxisAlignedBoundingBox & bounding,
		const PGeometryNodeList & geometry,
		const KDTreePlane *
	) const
	{
		const Float4 boundingExtents = extents(bounding);
		const ASizeT maxAxis = argmax3(boundingExtents);
		const PGeometryNodeList sortedGeometry = SamplingKDTreeBalancer::geometrySortByMinimum(maxAxis, geometry);
		const ASizeT geometrySize = sortedGeometry.size();
		assert(geometrySize > 1);

		if (geometrySize & One<ASizeT>()) {
			// list-size isn't even, median is at "geometrySize / 2 + 1"
			return SamplingKDTreeBalancer::sampleSplittingPlane(
				maxAxis,
				bounding.minimum,
				boundingExtents,
				SamplingKDTreeBalancer::geometryNodeMinimumPredicate(*sortedGeometry[(geometrySize >> 1) + One<ASizeT>()])
			);
		}

		// list-size is even, median is average of nodes at "geometrySize / 2" and "geometrySize / 2 + 1"
		const ASizeT geometrySizeHalf = geometrySize >> 1;
		const Float4 medianCoords = Half<Float>() *
			(SamplingKDTreeBalancer::geometryNodeMinimumPredicate(*sortedGeometry[geometrySizeHalf]) +
				SamplingKDTreeBalancer::geometryNodeMinimumPredicate(*sortedGeometry[geometrySizeHalf + One<ASizeT>()]));

		return SamplingKDTreeBalancer::sampleSplittingPlane(maxAxis, bounding.minimum, boundingExtents, medianCoords);
	}

}
