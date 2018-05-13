#pragma once

#include "geometry/partitioning/GeometryNode.h"
#include "geometry/partitioning/KDTreePlane.h"

namespace raytracer
{

	struct KDTreeBounding
	{
		static void split(const KDTreePlane & splitter, const AxisAlignedBoundingBox & toSplit, AxisAlignedBoundingBox & leftBounding, AxisAlignedBoundingBox & rightBounding);

		static const AxisAlignedBoundingBox findMinimumBoundingOfGeometry(const PGeometryNodeList & geometry);
	};

}
