#include "../../stdafx.h"

#include "geometry/partitioning/KDTreeBounding.h"

#include <thread>

namespace raytracer
{

	void KDTreeBounding::split(
		const KDTreePlane & splitter,
		const AxisAlignedBoundingBox & toSplit,
		AxisAlignedBoundingBox & leftBounding,
		AxisAlignedBoundingBox & rightBounding)
	{
		leftBounding = AxisAlignedBoundingBox(
			toSplit.minimum,
			splitter.splitMax(toSplit.minimum, toSplit.maximum)
		);
		rightBounding = AxisAlignedBoundingBox(
			splitter.splitMin(toSplit.minimum, toSplit.maximum),
			toSplit.maximum
		);
	}

	const AxisAlignedBoundingBox KDTreeBounding::findMinimumBoundingOfGeometry(const PGeometryNodeList & geometry)
	{
		const int geometrySize = static_cast<int>(geometry.size());
		AxisAlignedBoundingBox bounding = AxisAlignedBoundingBox();

#pragma omp parallel if (geometrySize >= int(std::thread::hardware_concurrency()))
		{
			AxisAlignedBoundingBox t = AxisAlignedBoundingBox();

#pragma omp for nowait
			for (int i = Zero<int>(); i < geometrySize; ++i)
			{
				geometry[static_cast<ASizeT>(i)]->includeInBounding(t);
			}

#pragma omp critical (mergeBoundings)
			{
				bounding = extendBy(t, bounding);
			}
		}

		return bounding;
	}

}
